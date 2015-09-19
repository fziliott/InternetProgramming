class PrintThread extends Thread {
    private String message;
    private static Object lock = new Object(); //The same for every PrintThread

    PrintThread(String msg) {
        message = msg;
    }

    public void run() {
        for (int i = 0; i < 10 ; i++) {
            synchronized(lock) {        //Only one PrintThread at a time can call syn1.display()
                syn1.display(message);
            }
        }
    }
}

public class syn1 {
    public static void main(String[] args) {

        PrintThread t1 = new PrintThread("Hello_world\n");
        PrintThread t2 = new PrintThread("Bonjour_monde\n");

        t1.start();
        t2.start();
    }

    public static void display(String str) {
        for(int i = 0; i < str.length(); i++) {
            System.out.print(str.charAt(i));
        }
    }
}