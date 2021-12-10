import connection.Game;

public class Main {
    public static void main(String[] args) {

        final String severName = "127.0.0.1";
        final int serverPort = 4444;
        Game game = new Game();

        game.connectToServer(severName, serverPort);
        game.responseFromServer();
    }
}
