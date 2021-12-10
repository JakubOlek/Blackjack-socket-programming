package request;

import java.io.IOException;
import java.net.Socket;

interface RequestInterface {
    void sendMessage(Socket socket, String message) throws IOException;
}
