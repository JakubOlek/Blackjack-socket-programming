package request;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.Socket;

public class Request implements RequestInterface {

    @Override
    public void sendMessage(Socket socket, String message) throws IOException {

        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));

        try {
            writer.write(message);
            writer.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
