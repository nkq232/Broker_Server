import org.json.JSONObject;

public class Main {
    public static void main(String[] args) {
        Sensor temperature = new Sensor("24", "8", "localhost");
        temperature.sendData();
    }
}
