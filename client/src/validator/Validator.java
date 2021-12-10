package validator;


public class Validator {
    public static boolean validateInput(String message) {
        try {
            return message.equals("STAND") || message.equals("HIT");

        } catch (Exception e) {
            return false;
        }
    }
}
