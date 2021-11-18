//Nguyễn Khánh Quân 19020400
//Week 7 Client
//Truyền file client server java
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.ServerSocket;
import java.net.*;
import java.io.*;
import java.util.Scanner;
import java.io.File;
public class client{
	public static void main(String[] args){
		final String serverHost = args[0];

		Socket clientSocket = null;
		BufferedReader rd;
		BufferedWriter wt;

		try {
			clientSocket = new Socket(serverHost,8060);
			rd = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			wt = new BufferedWriter(new OutputStreamWriter(clientSocket.getOutputStream()));
		} catch (UnknownHostException e) {
           System.err.println("Don't know about host " + serverHost);
           return;
       } catch (IOException e) {
           System.err.println("Couldn't get I/O for the connection to " + serverHost);
           return;
       }
       
       	try{
			Scanner in = new Scanner(System.in);
       		while(true){
		    String message, response, username, userage;
		    System.out.print("Writing message: ");
		 	message = in.nextLine();
		    wt.write(message);
		    wt.newLine();
		    wt.flush();
		    response = rd.readLine();
		    System.out.println("From server: " + response);
		    if (message.equals("USER INFO")) {
	       		System.out.print("Input your user name: ");
	       		username = in.nextLine();
	       		System.out.print("Input your user age: ");
	       		userage = in.nextLine();
        		String new_message = "{ “User name”: “" + username + "”, “User age”: “" + userage + "”}";
        		wt.write(new_message);
	       		wt.newLine();
	       		wt.flush();
	       		response = rd.readLine();
	       		System.out.println("From server: " + response);
	       } else if (message.equals("DOWNLOAD")) {
	       		System.out.print("File name: ");
	       		message = in.nextLine();
	       		wt.write(message);
	       		wt.newLine();
	       		wt.flush();

	       		response = rd.readLine();

	       		if (response.equals("305 File Not Found")) {
	       			System.out.println("From Server: " + response);
	       		} else {
	       			System.out.println("From Server: " + response);

	       			response = rd.readLine();
	       			long length = Long.parseLong(response);
	       			response = rd.readLine();
	       			System.out.println("From Server: " + response);
	       			String messages = "OK";
	       			wt.write(messages);
	       			wt.newLine();
	       			wt.flush();
	       			byte[] contents;
	       			
	       			FileOutputStream fos = new FileOutputStream(message);
	       			BufferedOutputStream bos = new BufferedOutputStream(fos);
	       			InputStream is = clientSocket.getInputStream();
	       			long i = 0;
	       			while(i != length) {
						int size = 10000;
					 	if (length - i >= size) {
					 		i+= size;
					 	} else {
					 		size = (int)(length - i);
					 		i = length;
					 	}
					 	contents = new byte[size];
						is.read(contents, 0, size);
						bos.write(contents);
					}
					bos.close();
					response = rd.readLine();
	       			System.out.println("From Server: " + response);
	       		}
	       }
	        else if (message.equals("QUIT")) {
	       		rd.close();
        		wt.close();
        		clientSocket.close();
				
        		break;
	       		}
	    	}
			in.close();
    	} catch (UnknownHostException e) {
           System.err.println("Trying to connect to unknown host: " + e);
       } catch (IOException e) {
           System.err.println("IOException:  " + e);
       }
	}
}