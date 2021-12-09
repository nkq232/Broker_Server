import org.json.JSONObject;

import java.io.*;
import java.net.Socket;
import java.sql.Time;
import java.util.Random;
import java.util.concurrent.TimeUnit;

public class Sensor {
    public static final int SERVER_PORT = 6060;
    private final Socket socket;
    private final String location;
    private final String type;
    private final byte[] readBuff = new byte[1024];
    public Sensor(String location, String type, String address) {
        this.location = location;
        this.type = type;
        socket = createSocket(address);
    }

    private Socket createSocket(String address) {
        try {
            return new Socket(address, SERVER_PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    private void sendOneMessage(OutputStream out, InputStream in, String message) throws java.io.IOException {
        out.write(message.getBytes());
        int readLen = in.read(readBuff);
        String readMess = new String(readBuff, 0, readLen);
        System.out.println("Server response: " + readMess);
    }

    public void sendData() {
        try {
            OutputStream out = new DataOutputStream(socket.getOutputStream());
            InputStream in = new DataInputStream(socket.getInputStream());

            sendOneMessage(out, in, "SENSOR HELO Server");
            Random rd = new Random();
            while (true) {
                sendOneMessage(out, in, "INFO");
                JSONObject info = new JSONObject();
                info.put("TypeID", type);
                info.put("LocationID", location);
                info.put("Value", 25 + rd.nextInt(5));
                sendOneMessage(out, in, info.toString('\t'));
                TimeUnit.SECONDS.sleep(5);
            }
//            sendOneMessage(out, in, "QUIT");
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
