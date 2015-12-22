using UnityEngine;
using System.Collections;

public class cloth_motion: MonoBehaviour {

	float 		t;				// The time step
	Vector3[] 	velocities;		// The velocity array
	float		damping;		// The damping multiplier coefficient
	int[] 		edge_list;		// The edge list
	float[] 	L0;				// The edge rest length list
	float		gravity;		// gravity force


	// Use this for initialization
	void Start () 
	{
		t 			= 0.075f;
		damping 	= 0.99f;
		gravity 	= -.8f;

		Mesh mesh = GetComponent<MeshFilter> ().mesh;
		int[] 		triangles = mesh.triangles;
		Vector3[] 	vertices = mesh.vertices;

		//Construct the original edge list
		int[] original_edge_list = new int[triangles.Length*2];
		for (int i=0; i<triangles.Length; i+=3) 
		{
			original_edge_list[i*2+0]=triangles[i+0];
			original_edge_list[i*2+1]=triangles[i+1];
			original_edge_list[i*2+2]=triangles[i+1];
			original_edge_list[i*2+3]=triangles[i+2];
			original_edge_list[i*2+4]=triangles[i+2];
			original_edge_list[i*2+5]=triangles[i+0];
		}
		//Reorder the original edge list
		for (int i=0; i<original_edge_list.Length; i+=2)
			if(original_edge_list[i] > original_edge_list[i + 1]) 
				Swap(ref original_edge_list[i], ref original_edge_list[i+1]);
		//Sort the original edge list using quicksort
		Quick_Sort (ref original_edge_list, 0, original_edge_list.Length/2-1);

		int count = 0;
		for (int i=0; i<original_edge_list.Length; i+=2)
			if (i == 0 || 
			    original_edge_list [i + 0] != original_edge_list [i - 2] ||
			    original_edge_list [i + 1] != original_edge_list [i - 1]) 
					count++;

		edge_list = new int[count * 2];
		int r_count = 0;
		for (int i=0; i<original_edge_list.Length; i+=2)
			if (i == 0 || 
			    original_edge_list [i + 0] != original_edge_list [i - 2] ||
				original_edge_list [i + 1] != original_edge_list [i - 1]) 
			{
				edge_list[r_count*2+0]=original_edge_list [i + 0];
				edge_list[r_count*2+1]=original_edge_list [i + 1];
				r_count++;
			}


		L0 = new float[edge_list.Length/2];
		for (int e=0; e<edge_list.Length/2; e++) 
		{
			int v0 = edge_list[e*2+0];
			int v1 = edge_list[e*2+1];
			L0[e]=(vertices[v0]-vertices[v1]).magnitude;
		}

		velocities = new Vector3[vertices.Length];
		for (int v=0; v<vertices.Length; v++)
			velocities [v] = new Vector3 (0, 0, 0);

	}

	void Quick_Sort(ref int[] a, int l, int r)
	{
		int j;
		if(l<r)
		{
			j=Quick_Sort_Partition(ref a, l, r);
			Quick_Sort (ref a, l, j-1);
			Quick_Sort (ref a, j+1, r);
		}
	}

	int  Quick_Sort_Partition(ref int[] a, int l, int r)
	{
		int pivot_0, pivot_1, i, j;
		pivot_0 = a [l * 2 + 0];
		pivot_1 = a [l * 2 + 1];
		i = l;
		j = r + 1;
		while (true) 
		{
			do ++i; while( (a[i*2]<pivot_0 || a[i*2]==pivot_0 && a[i*2+1]<=pivot_1) && i<=r);
			do --j; while(  a[j*2]>pivot_0 || a[j*2]==pivot_0 && a[j*2+1]> pivot_1);
			if(i>=j)	break;
			Swap(ref a[i*2], ref a[j*2]);
			Swap(ref a[i*2+1], ref a[j*2+1]);
		}
		Swap (ref a [l * 2 + 0], ref a [j * 2 + 0]);
		Swap (ref a [l * 2 + 1], ref a [j * 2 + 1]);
		return j;
	}

	void Swap(ref int a, ref int b)
	{
		int temp = a;
		a = b;
		b = temp;
	}

	void Strain_Limiting(ref Vector3[] vertices){

		// Initialize temporary arrays and counter
		Vector3[] temp_x = new Vector3[vertices.Length];
		int[] temp_n = new int[vertices.Length];
		Vector3 x_new = new Vector3 (0, 0, 0);
		Vector3 xie = new Vector3 (0, 0, 0);
		Vector3 xje = new Vector3 (0, 0, 0);
		int i, j;
		
		// Loop through each edge getting the vertices of the edges
		for (int e = 0; e < edge_list.Length/2; e++){

			// Get edge point for vertices
			i = edge_list[2*e + 0];
			j = edge_list[2*e + 1];

			// Get desired position of vertex i and j
			xie = 0.5f * (vertices[i] + vertices[j] + L0[e] * ((vertices[i] - vertices[j])/(vertices[i] - vertices[j]).magnitude));
			xje = 0.5f * (vertices[i] + vertices[j] + L0[e] * ((vertices[j] - vertices[i])/(vertices[j] - vertices[i]).magnitude));

			// Put desired vertexes into temporary array
			temp_x[i] += xie;
			temp_x[j] += xje;

			// Increment
			temp_n[i]++;
			temp_n[j]++;

		}

		// Update velocity and vertices with recalculated values
		for (int p = 1; p < vertices.Length; p++) {
			if (p != 10) {
				x_new = (2 * vertices[p] + temp_x[p])/(2 + temp_n[p]);
				velocities[p] += (1/t * (x_new - vertices[p]));
				vertices[p] = x_new;
			}
		}
	}

	void Collision_Handling(ref Vector3[] vertices) {

		// Initialize radius, sphere, center
		Vector3 x_new = new Vector3 (0, 0, 0);
		GameObject sphere = GameObject.Find ("Sphere");
		Vector3 center = sphere.transform.position;
		float R = sphere.transform.localScale.magnitude*0.3f;

		// Apply collision handling on cloth and sphere
		for (int i = 0; i < vertices.Length; i++) {
			if ((vertices[i]-center).magnitude <= R) {
				x_new = center + R*((vertices[i]-center)/(vertices[i]-center).magnitude);
				velocities[i] += (x_new - vertices[i])/t;
				vertices[i] = x_new;
			}
		}
	}

	// Update is called once per frame
	void Update () 
	{
		// Use t as the time step (Delta t).
		// Edge_list and L0 have both been created already.
		// Your job is to finish the rest of the code.

		Mesh mesh = GetComponent<MeshFilter> ().mesh;
		Vector3[] vertices = mesh.vertices;


		//Step 1: Formulate the simulation as a basic particle system with the gravity acceleration only.

		// Iterate through vertices applying gravity and damping to velocities
		for (int i = 1; i < vertices.Length; i++) {

			// Skip vertices 0 and 10 to hold up cloth
			if (i != 10){
				velocities[i].y += t * gravity;
				velocities[i].y *= damping;
				vertices[i] += t * velocities[i];
			}
		}

		//Step 2: Apply the strain limiting method as a constraint
		//You may apply it multiple times here. (64 times, for example)

		for (int i = 1; i < 64; i++){
			Strain_Limiting(ref vertices);
		}

		//Step 3: Apply sphere-vertex collision as a constraint

		Collision_Handling (ref vertices);

		mesh.vertices = vertices;
		mesh.RecalculateNormals ();

	}
}
