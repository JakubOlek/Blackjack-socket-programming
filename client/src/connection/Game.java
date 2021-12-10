package connection;

import request.Request;
import validator.Validator;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.Socket;
import java.util.Scanner;

public class Game {

    public Socket socket;
    public String message;
    Request request = new Request();
    Scanner userInput = new Scanner(System.in);

    public void connectToServer(String severName, int serverPort) {
        try {
            socket = new Socket(severName, serverPort);
            System.out.println("Connected\n");

        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
            System.exit(1);
        }
    }

    public void responseFromServer() {
        try {
            BufferedReader messageFromServer = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            String readMessage = "";
            do {
                readMessage = messageFromServer.readLine();
                System.out.println(readMessage);

                if (readMessage.equals("Type HIT/STAND")) {
                    message = userInput.nextLine().toUpperCase();

                    while (!Validator.validateInput(message)) {
                        System.out.println("Incorrect input. Type HIT/STAND:");
                        message = userInput.nextLine().toUpperCase();
                    }
                    request.sendMessage(socket, message);
                }

            } while (!readMessage.equals("Thanks for the game!"));

        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
        }
    }
}
