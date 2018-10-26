//import java.util.Random;
import java.io.*;
import java.util.Arrays;

public class pmtest {

    native String[] init();
    native String[] yes();
    native String[] no();
    
    public static void main(String[] args) {
	System.loadLibrary("pm");
	int decision;
	//Random rand = new Random();
	pmtest test = new pmtest();
	String[] decision_info = new String[4];
	System.out.println("Initializing");
	decision_info = test.init();
	System.out.println("loop");
	int x = 4;
	while(true) {
	    decision  = db_fun(decision_info[0], decision_info[1], decision_info[2], decision_info[3]);
	    //System.out.println(decision);
	    /*for (int i = 0; i < 4; i++) 
	      System.out.println(decision_info[i]);*/
	    if (decision == 1) {
		//System.out.println("yes");
		decision_info = test.yes();
	    }
	    else {
		//System.out.println("no");    
		decision_info = test.no();
	    }
	    x--;
	}
    }

    public static int db_fun(String process_id, String user_id, String pathname, String system_call) {
	System.out.println("db_fun");
	try {
	File file = new File("db.txt");
	FileReader fileReader = new FileReader(file);
	BufferedReader bufferedReader = new BufferedReader(fileReader);
	String line;

	while ((line = bufferedReader.readLine()) != null && !line.contains(system_call))
	    System.out.println(line);

	while ((line = bufferedReader.readLine()) != null && !line.contains(pathname));

	//bufferedReader.readLine();

	if((line = bufferedReader.readLine()).contains(user_id)) {
	    fileReader.close();
	    return 1;
	}

	fileReader.close();
	return 0;
	}
	catch (IOException e) {
	    e.printStackTrace();
	}
	
	return -1;
    }

	    
	
}
