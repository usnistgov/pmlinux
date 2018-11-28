//import java.util.Random;

public class pmtest {

    native String[] init();
    native String[] yes();
    native String[] no();
    
    public static void main(String[] args) {
	System.loadLibrary("pm");
	int decision = 0;
	//Random rand = new Random();
	pmtest test = new pmtest();
	System.out.println("Initializing");
	String[] decision_info = test.init();
	System.out.println("loop");
	int x = 4;
	while(x>=0) {
	    //System.out.println(decision);
	    for (String info:decision_info) {
		    System.out.println(info);
	    }
	    if (decision == 1) {
		decision = 0;
		//System.out.println("yes");
		decision_info = test.yes();
	    }
	    else {
		decision = 1;
		//System.out.println("no");    
		decision_info = test.no();
	    }
	    //x--;
	}
    }
}
