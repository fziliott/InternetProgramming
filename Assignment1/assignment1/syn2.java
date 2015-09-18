class PrintThread2 extends Thread {
    private String message;
    private Boolean canWrite;
    public PrintThread2 otherThread;
    private static Object lock = new Object();

    PrintThread2(String msg, Boolean cWrite) {
        message = msg;
        canWrite = cWrite;
    }

    public void run() {
        for (int i = 0; i < 10 ; i++) {
            synchronized(lock) {
                while(!canWrite.booleanValue()) { //check if it's my turn to write (an if statement would be enough because there are only two threads and the condition, the while is needed when there is the possibility of getting notified without getting the lock)
                    try {
                        lock.wait();
                    } catch(InterruptedException ex) {
                        System.out.println("Thread was interrupted!");
                    }
                }
                syn2.display(message);
                canWrite = new Boolean (false);  //Now this thread can't write until the other has written its message
                otherThread.canWrite = new Boolean(true); //Allows the other thread to write
                lock.notifyAll();  //Wakes up the other thread
            }
        }
    }
}

public class syn2 {
    public static void main(String[] args) {
        int i = 0;

        Boolean writeAb =  new Boolean(true);
        Boolean  writeCd = new Boolean(false);

        PrintThread2 t1 = new PrintThread2("ab", writeAb);
        PrintThread2 t2 = new PrintThread2("cd\n", writeCd);
        t1.otherThread = t2;
        t2.otherThread = t1;
        t1.start();
        t2.start();
    }

    public static void display(String str) {
        for(int i = 0; i < str.length(); i++) {
            System.out.print(str.charAt(i));
        }
    }
}