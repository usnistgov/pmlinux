import java.io.*;
public class test {
    public static void main(String[] args) {
	int res = db_fun("1001", "1001", "test.txt", "open");
	System.out.println(res);
    }
public static int db_fun(String process_id, String user_id, String pathname, String system_call) {
    System.out.println("db_fun");
    try {                                                                                                                                                                                           
        File file = new File("db.txt");                                                                                                                                                                  
        FileReader fileReader = new FileReader(file);                                                                                                                                                    
        BufferedReader bufferedReader = new BufferedReader(fileReader);                                                                                                                                  
        String line;
 
        while ((line = bufferedReader.readLine()) != null && !(line.contains(system_call)));
	System.out.println(line);
        while ((line = bufferedReader.readLine()) != null && !line.contains(pathname));                                                                                                                  

	System.out.println(line);
        //line = bufferedReader.readLine();                                                                                                                                                                       
	System.out.println(line);
        if((line = bufferedReader.readLine()).contains(user_id)) {                                                                                                                                       
	fileReader.close();
	System.out.println(line);
	return 1;                                                                                                                                                                                     
        }                                                                                                                                                                                                
                                                                                                                                                                                                         
        fileReader.close();
	System.out.println(line);
        return 0;                                                                                                                                                                                        
        }               
        catch (IOException e) {
	e.printStackTrace();
	}
    return -1;
}
}
