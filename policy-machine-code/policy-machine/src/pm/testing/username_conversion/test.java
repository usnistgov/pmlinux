import java.io.*;

public class test {
    native String get_username(String user_id);

    public static void main(String[] args) {
	System.loadLibrary("pm");

	test test = new test();
	String username = test.get_username("1001");

	System.out.println(username);
    }

}
