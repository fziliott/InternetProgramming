class PrintMonitor {
    private boolean writeAb = true;

    synchronized public void printAb() throws InterruptedException {
        while(!writeAb) {
            wait();
        }
        display("ab");
        writeAb = false;
        notifyAll();
    }
    synchronized public void printCd() throws InterruptedException {
        while(writeAb) {
            wait();
        }
        display("cd\n");
        writeAb = true;
        notifyAll();
    }

    public static void display(String str) {
        for(int i = 0; i < str.length(); i++) {
            System.out.print(str.charAt(i));
        }
    }
}

class PrintThreadAb extends Thread {
    PrintMonitor monitor;
    PrintThreadAb(PrintMonitor m) {
        monitor = m;
    }

    public void run() {
        for (int i = 0; i < 10 ; i++) {
            try {
                monitor.printAb();
            } catch(InterruptedException ext) {
                System.out.println("PrintThreadAb was interrupted!");
                ext.printStackTrace();
            }
        }
    }
}

class PrintThreadCd extends Thread {
    PrintMonitor monitor;
    PrintThreadCd(PrintMonitor m) {
        monitor = m;
    }
    
    public void run() {
        for (int i = 0; i < 10 ; i++) {
            try {
                monitor.printCd();
            } catch(InterruptedException ext) {
                System.out.println("PrintThreadCd was interrupted!");
                ext.printStackTrace();
            }
        }
    }
}

public class syn2 {
    public static void main(String[] args) throws InterruptedException {
        PrintMonitor monitor = new PrintMonitor();
        PrintThreadAb tAb = new PrintThreadAb(monitor);
        PrintThreadCd tCd = new PrintThreadCd(monitor);
        tAb.start();
        tCd.start();
    }
}