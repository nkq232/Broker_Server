import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.io.File;
import java.io.*;

public class multiThread implements Runnable{
	private Socket serverSocket;
	public multiThread (Socket serverSocket) {
		this.serverSocket = serverSocket;
	}  
	public void run() {
		BufferedReader rd;
		BufferedWriter wt;
		System.out.println("Client connect !");
		try {
			rd = new BufferedReader(new InputStreamReader(serverSocket.getInputStream()));
			wt = new BufferedWriter(new OutputStreamWriter(serverSocket.getOutputStream()));
			commute(wt, rd, serverSocket);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	public static void commute(BufferedWriter wt, BufferedReader rd, Socket serverSocket) {
		while(true) {
			try {
				String input;
				String output;
				input = rd.readLine();
				System.out.println("From client: " + input);
				if (input.equals("HELO Server"))
			{
				/* code */
				output= "200 Hello Client";
				wt.write(output);
				wt.newLine();
				wt.flush();
				System.out.println("Sending to client: 200 Hello Client\n");
			}
				else if (input.equals("USER INFO"))
			{
				/* code */
				output= "210 OK";
				wt.write(output);
				wt.newLine();
				wt.flush();
				System.out.println("Sending to client: 210 OK");
				String username = "";
				String userage = "";
				input = rd.readLine();
				int count1 = 0;
				int count2 = 0;
				for (int i = 0; i < input.length(); i++) {
					if (input.charAt(i) == '“') {
						count1 ++;
					}
					if (input.charAt(i) == '”') {
						count2 ++;
					}
					if (count1 == 2 && count2 == 1) {
						username += input.charAt(i);
					}
					if (count1 == 4 && count2 == 3) {
						userage += input.charAt(i);
					}
					if (count2 == 4) {
						break;
					}
				}
				
				username = username.substring(1);
				userage = userage.substring(1);

				System.out.println("From Client: " + input);
				int check = 0;
				for (int i = 0; i < username.length(); i++)
				{
					/* code */
					if (!((username.charAt(i) >= 'A' && username.charAt(i) <= 'Z') || (username.charAt(i) >= 'a' && username.charAt(i) <= 'z')))
					{
						/* code */
						if (username.charAt(i) == '\0' || username.charAt(i) == '\n')
						{
							/* code */
							continue;
						}
						check++;
						break;
					}
				}
				if (check == 0) {
					for (int i = 0; i < userage.length(); ++i)
				{
					/* code */
					if (!(userage.charAt(i) >= '0' && userage.charAt(i) <= '9'))
					{
						/* code */
						if (userage.charAt(i) == '\0' || userage.charAt(i) == '\n')
						{
							/* code */
							continue;
						}
						check++;
						break;
					}
				}
				}
				if (check == 0)
				{
					/* code */
					output = "211 User Info OK";
					wt.write(output);
					wt.newLine();
					wt.flush();
					System.out.println("Sending to client: 211 User Info OK");
				} else {
					output = "400 User Info Error";
					wt.write(output);
					wt.newLine();
					wt.flush();
					System.out.println("Sending to client: 400 User Info Error");
				}
			}
				else if (input.equals("QUIT"))
			{
				/* code */
				output= "500 bye";
				wt.write(output);
				wt.newLine();
				wt.flush();
				System.out.println("Sending to client: " + output);
				break;
			}
				else if (input.equals("DOWNLOAD")) {
					output = "300 Download OK";
					wt.write(output);
					wt.newLine();
					wt.flush();
					System.out.println("Sending to client: " + output);
					input = rd.readLine();

					File file = new File(input);
					if (file.exists()) {
						System.out.println("Sending to client: 310 " + input +" File Found");
						wt.write("310 " + input + " File Found");
						wt.newLine();
						wt.flush();

						System.out.println("File name: " + input);
						System.out.println("File size: " + file.length());
						String a = "" + file.length();
						wt.write(a);
						wt.newLine();
						wt.flush();
						a = "File size: " + file.length() + " bytes";
						wt.write(a);
						wt.newLine();
						wt.flush();
						input = rd.readLine();
						FileInputStream fis = new FileInputStream(file);
						BufferedInputStream bis = new BufferedInputStream(fis);
						OutputStream os = serverSocket.getOutputStream();
						byte[] contents;
						long length = file.length();
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
							bis.read(contents, 0, size);
							os.write(contents);
						}
						os.flush();
						bis.close();
						a = "Finish File Transfer";
						wt.write(a);
						wt.newLine();
						wt.flush();
						}
					else {
						output = "305 File Not Found";
						wt.write(output);
						wt.newLine();
						wt.flush();
						System.out.println("Sending to client: " + output);
						
					}
				}
				else {
					output = "600 Wrong Syntax";
					wt.write(output);
					wt.newLine();
					wt.flush();
					System.out.println("Sending to client: 600 Wrong Syntax");
				}
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
  	}
}