//import java.util.Random;

public class pmtest {

    native void init();
    native String[] yes();
    native String[] no();
    
    public static void main(String[] args) {
	System.loadLibrary("pm");
	int decision = 1;
	//Random rand = new Random();
	pmtest test = new pmtest();
	String[] decision_info = new String[4];
	System.out.println("Initializing");
	test.init();
	System.out.println("loop");
	int x = 4;
	while(x>=0) {
	    //System.out.println(decision);
	    //for (int i = 0; i < 4; i++) 
		//System.out.println(decision_info[i]);
	    if (decision == 1) {
		//decision = 0;
		//System.out.println("yes");
		decision_info = test.yes();
	    }
	    else {
		//decision = 1;
		//System.out.println("no");    
		decision_info = test.no();
	    }
	    //x--;
	}
    }
}
