import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.Semaphore;

public class ThreadExample {
    private static final int ui_Size = 100;
    private static int[] i_Buffer1 = new int[ui_Size];
    private static int[] i_Buffer2 = new int[ui_Size];

    private static Lock tu_Mutex = new ReentrantLock();
    private static Semaphore tu_Semaphore1 = new Semaphore(0);
    private static Semaphore tu_Semaphore2 = new Semaphore(0);

    public static void main(String[] args) {
        Thread ul_Thread1 = new Thread(new Thread1());
        Thread ul_Thread2 = new Thread(new Thread2());
        Thread ul_Thread3 = new Thread(new Thread3());

        ul_Thread1.start();
        ul_Thread2.start();
        ul_Thread3.start();
    }

    static class Thread1 implements Runnable {
        @Override
        public void run() {
            int ui_I, ui_J, ui_Loopcounter;
            int[] i_Array1 = new int[4];
            ui_J = 0;
            ui_Loopcounter = 0;

            while (true) {
                for (ui_I = 0; ui_I < 4; ui_I++) {
                    i_Array1[ui_I] = ui_Loopcounter;
                }

                tu_Mutex.lock();

                for (ui_I = 0; ui_I < 4; ui_I++) {
                    i_Buffer1[ui_J] = i_Array1[ui_I];
                    ui_J++;
                }

                tu_Mutex.unlock();

                if (ui_J >= ui_Size) {
                    ui_J = 0;
                }

                System.out.println("LoopCounter of thread A: " + ui_Loopcounter);
                ui_Loopcounter++;

                try {
                    Thread.sleep(250);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                tu_Semaphore1.release();
            }
        }
    }

    static class Thread2 implements Runnable {
        @Override
        public void run() {
            int ui_I, ui_J, ui_Loopcounter;
            int[] i_Array2 = new int[4];
            ui_J = 0;
            ui_Loopcounter = 0;

            while (true) {
                try {
                    tu_Semaphore1.acquire();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                tu_Mutex.lock();

                for (ui_I = 0; ui_I < 4; ui_I++) {
                    i_Array2[ui_I] = i_Buffer1[ui_J];
                    i_Buffer2[ui_J] = i_Array2[ui_I];
                    ui_J++;
                }

                tu_Mutex.unlock();

                if (ui_J >= ui_Size) {
                    ui_J = 0;
                }

                for (ui_I = 0; ui_I < 4; ui_I++) {
                    System.out.println("| Array from A to B: " + i_Array2[ui_I] + "|");
                }

                System.out.println("LoopCounter of thread B: " + ui_Loopcounter);
                ui_Loopcounter++;

                try {
                    Thread.sleep(250);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                tu_Semaphore2.release();
            }
        }
    }

    static class Thread3 implements Runnable {
        @Override
        public void run() {
            int ui_I, ui_J, ui_Loopcounter;
            int[] i_Array3 = new int[4];
            ui_J = 0;
            ui_Loopcounter = 0;

            while (true) {
                try {
                    tu_Semaphore2.acquire();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                tu_Mutex.lock();

                for (ui_I = 0; ui_I < 4; ui_I++) {
                    i_Array3[ui_I] = i_Buffer2[ui_J];
                    ui_J++;
                }

                tu_Mutex.unlock();

                if (ui_J >= ui_Size) {
                    ui_J = 0;
                }

                for (ui_I = 0; ui_I < 4; ui_I++) {
                    System.out.println("| Array from B to C: " + i_Array3[ui_I] + "|");
                }

                System.out.println("LoopCounter of thread C: " + ui_Loopcounter);
                ui_Loopcounter++;

                try {
                    Thread.sleep(250);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
