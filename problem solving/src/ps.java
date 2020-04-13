import java.util.Scanner;

public class ps {

	public static int  maxelement(int arr[]) {

		int max = arr[0];
		int index = 0;
		for (int i=0;i<arr.length-1;i++) {
			if(arr[i+1]>arr[i]) {
				max=arr[i+1];
		     	index=i+1;
		}
		}
		for(int i=index;i<arr.length-1;i++) {
			arr[i]=arr[i+1];	
		}
		arr[arr.length-1]=0;
		return max;
		
	}
	public static int  minelement(int arr[]) {

		int min = arr[0];
		int index = 0;

		for (int i=0;i<arr.length-2;i++) {
			if(arr[i+1]<arr[i]) { 
				min=arr[i+1];
	     	    index=i+1;
			}
		}
		for(int i=index;i<arr.length-2;i++) {
			arr[i]=arr[i+1];	
		}
		arr[arr.length-2]=0;
		return min;
		
	}
	
	public static void main(String[] args) {

		Scanner input=new Scanner(System.in);
		
		int q,size,k = 0;
		System.out.println("enter the size of the array");
		size=input.nextInt();
		int mainarray[]=new int[size];
		System.out.println("enter the number of quieries");
		q=input.nextInt();
		System.out.println("enter the elements of the array");
		for(int i=0;i<size;i++) {
		mainarray[i]=input.nextInt();
		   }		
			
		for(int i=0;i<q;i++) {
			int aftercopy[] = null;
			System.out.println("enter k");
			k=input.nextInt();
			int copy[]=new int[size];
			for (int j=0;j<size;j++) {
				copy[j]=mainarray[j];
			}
			int dsize=size;
			for(int w=0;w<k;w++) {
				
			int max=maxelement(copy);
			int min=minelement(copy);
			int result=max-min;
			dsize--;
			
			
			}
			int sum=0;
			for(int h=0;h<copy.length;h++) {
				sum+=copy[h];
			}
			System.out.println(sum);
		}
		
		
		
		
		
	}

}
