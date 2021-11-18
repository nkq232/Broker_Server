import java.io.IOException;
import java.net.Socket;
import java.net.ServerSocket; 
public class server {
	private static int threadNumber = 5;
	public static void main(String[] args) {
		ServerSocket listener = null;
		Socket serverSocket = null;
		try {
			listener = new ServerSocket(8060);
		} catch (IOException e){
			System.out.println(e);
			System.exit(1);
		}

		try {
			for (int i = 0; i < threadNumber; i++) {		
			System.out.println("Waiting for client .");
			serverSocket = listener.accept();
			multiThread index = new multiThread(serverSocket);
			Thread myThread = new Thread(index);
			myThread.start();
			}	
		} catch (IOException e) {
	        System.out.println(e);
	        e.printStackTrace();
	    }
       	System.out.println("Server stopped!");

			
  	}
}