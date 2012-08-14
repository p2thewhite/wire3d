using System.Collections.Generic;

public class ListUtil
{
	
	public static void Reserve<T> (List<T> arg0, int arg1, T arg3)
	{
		for (int i = 0; i < arg1; i++) {
			arg0.Add (arg3);
		}
	}
	
	public static void ReserveList<T> (List<List<T> > arg0, int arg1)
	{
		for (int i = 0; i < arg1; i++) {
			arg0.Add (new List<T> ());
		}
	}
	
}
