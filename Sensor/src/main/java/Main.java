import org.json.JSONObject;

public class Main {
    public static void main(String[] args) {
        Sensor temperature = new Sensor("Hanoi", "temperature", "localhost");
        temperature.sendData();
    }
}
