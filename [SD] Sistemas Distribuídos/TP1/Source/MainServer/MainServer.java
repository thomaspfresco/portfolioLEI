import java.net.*;
import java.io.*;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;
import java.util.concurrent.Semaphore;

class Queue {
    private static int front, rear, capacity;
    private static String queue[];

    Queue(int size) {
        front = rear = 0;
        capacity = size;
        queue = new String[capacity];
    }

    void queueEnqueue(String item) {
        if (capacity == rear) {
            System.out.printf("\nQueue is full\n");
            return;
        } else {
            queue[rear] = item;
            rear++;
        }
        return;
    }

    void queueDequeue() {
        if (front == rear) {
            System.out.printf("\nQueue is empty\n");
            return;
        }

        else {
            for (int i = 0; i < rear - 1; i++)
                queue[i] = queue[i + 1];
            if (rear < capacity)
                queue[rear] = "";
            rear--;
        }
        return;
    }

    void queueDisplay() {
        int i;
        if (front == rear) {
            System.out.printf("Queue is Empty\n");
            return;
        }

        for (i = front; i < rear; i++) {
            System.out.printf(" %s , ", queue[i]);
        }
        return;
    }

    boolean queueEmpty() {
        if (front == rear)
            return true;
        else
            return false;
    }

    String queueFront() {
        if (front == rear) {
            System.out.printf("Queue is Empty\n");
            return null;
        }
        return queue[front];
    }
}

public class MainServer {
    private static int serverPort = 6000;

    private static void readData() {

        // leitura do ficheiro database
        String currentpath = System.getProperty("user.dir");
        File db = new File(currentpath + "/data.txt");

        String line;
        int counter = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(db))) {
            while ((line = br.readLine()) != null) {
                if (counter % 3 == 0) {
                    String[] splitted = line.split(":");
                    File theDir = new File(splitted[0]);
                    if (!theDir.exists())
                        theDir.mkdirs();
                }
                counter++;
            }
        } catch (IOException ex) {
            System.out.format("I/O exception: ", ex);
        }
    }

    private static void heartbeat() {

        String currentpath = System.getProperty("user.dir");
        File db = new File(currentpath + "/config.txt");

        int porto = 0, tries = 0, time = 0;
        String ip = "";
        int counter = 0;

        String line;
        String[] splitted;
        try (BufferedReader br = new BufferedReader(new FileReader(db))) {
            while ((line = br.readLine()) != null) {
                if (counter == 0) {
                    splitted = line.split(":");
                    ip = splitted[0]; // ip do outro servidor
                    porto = Integer.parseInt(splitted[1]); // porto do outro servidor
                } else if (counter == 1) {
                    splitted = line.split(":");
                    tries = Integer.parseInt(splitted[0]);
                    time = Integer.parseInt(splitted[1]);
                }
                counter++;
            }
        } catch (IOException ex) {
            System.out.format("I/O exception: ", ex);
        }

        // new Thread() {
        // public void run() {
        int c = 0;
        while (true) {
            if (c >= tries)
                break;

            try {
                Thread.sleep(time);
            } catch (InterruptedException ex) {
                Thread.currentThread().interrupt();
            }

            try (DatagramSocket aSocket = new DatagramSocket(serverPort)) {
                String ping = "ping";
                byte[] m = ping.getBytes();

                InetAddress aHost = InetAddress.getByName(ip);
                DatagramPacket request = new DatagramPacket(m, m.length, aHost, porto);
                aSocket.send(request);

                byte[] buffer = new byte[1000];
                aSocket.setSoTimeout(time);
                DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
                aSocket.receive(reply);
                System.out.println("Recebeu: " + new String(reply.getData(), 0, reply.getLength()));

                c = 0;
            } catch (SocketException e) {
                System.out.println("Socket: " + e.getMessage());
            } catch (IOException e) {
                System.out.println("Nao obtive resposta: " + e.getMessage());
                c++;
            }
        }

        // }
        // }.start();
    }

    private static void recebeHeartbeat() {
        new Thread() {
            public void run() {
                String currentpath = System.getProperty("user.dir");
                File db = new File(currentpath + "/config.txt");

                int porto = 0;
                String ip = "";
                int counter = 0;

                String line;
                String[] splitted;
                try (BufferedReader br = new BufferedReader(new FileReader(db))) {
                    while ((line = br.readLine()) != null) {
                        if (counter == 0) {
                            splitted = line.split(":");
                            ip = splitted[0]; // ip do outro servidor
                            porto = Integer.parseInt(splitted[1]); // porto do outro servidor
                        }
                        counter++;
                    }
                } catch (IOException ex) {
                    System.out.format("I/O exception: ", ex);
                }

                while (true) {
                    try (DatagramSocket aSocket = new DatagramSocket(serverPort)) {
                        byte[] buffer = new byte[1000];
                        // aSocket.setSoTimeout(10000);
                        //System.out.println("Estou a espera de um ping");
                        DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
                        aSocket.receive(reply);
                        //System.out.println("Recebeu: " + new String(reply.getData(), 0, reply.getLength()));

                        String texto = "pong";
                        byte[] m = texto.getBytes();
                        InetAddress aHost = InetAddress.getByName(ip);
                        DatagramPacket request = new DatagramPacket(m, m.length, aHost, porto);
                        aSocket.send(request);
                    } catch (SocketException e) {
                        System.out.println("Socket: " + e.getMessage());
                    } catch (IOException e) {
                        System.out.println("IO: " + e.getMessage());
                    }
                }

            }
        }.start();

    }

    private static boolean startUp() {
        String currentpath = System.getProperty("user.dir");
        File db = new File(currentpath + "/config.txt");

        int porto = 0, tries = 0, time = 0;
        String ip = "";
        int counter = 0;

        String line;
        String[] splitted;
        try (BufferedReader br = new BufferedReader(new FileReader(db))) {
            while ((line = br.readLine()) != null) {
                if (counter == 0) {
                    splitted = line.split(":");
                    ip = splitted[0]; // ip do outro servidor
                    porto = Integer.parseInt(splitted[1]); // porto do outro servidor
                } else if (counter == 1) {
                    splitted = line.split(":");
                    tries = Integer.parseInt(splitted[0]);
                    time = Integer.parseInt(splitted[1]);
                }
                counter++;
            }
        } catch (IOException ex) {
            System.out.format("I/O exception: ", ex);
        }

        // tenta comunicar com o outro servidor
        for (int i = 0; i < tries; i++) {
            try (DatagramSocket aSocket = new DatagramSocket(serverPort)) {
                // try (Scanner sc = new Scanner(System.in)) {

                String ping = "ping";
                byte[] m = ping.getBytes();

                InetAddress aHost = InetAddress.getByName(ip);
                DatagramPacket request = new DatagramPacket(m, m.length, aHost, porto);
                aSocket.send(request);

                byte[] buffer = new byte[1000];
                aSocket.setSoTimeout(time);
                DatagramPacket reply = new DatagramPacket(buffer, buffer.length);
                aSocket.receive(reply);
                System.out.println("Recebeu: " + new String(reply.getData(), 0, reply.getLength()));

                return true;

            } catch (SocketException e) {
                System.out.println("Socket: " + e.getMessage());
            } catch (IOException e) {
                System.out.println("Nao obtive resposta: " + e.getMessage());
            }
        }

        return false;
    }

    private static void enviaFile(Queue q) {
        try (DatagramSocket aSocket = new DatagramSocket(5500)) {
            while (true) {

                if (q.queueEmpty()) {
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException ex) {
                        Thread.currentThread().interrupt();
                    }
                }

                else {
                    synchronized (q) { // synchronized block
                        String filepath = q.queueFront();
                        InputStream in = new FileInputStream(filepath);

                        File f = new File(filepath);

                        String size = Integer.toString((int)(Math.ceil(f.length()/1024.0))); // tamanho de cada pacote

                        String texto = filepath + " " + size;
                        byte[] m = texto.getBytes();

                        InetAddress aHost = InetAddress.getByName("localhost");

                        DatagramPacket request = new DatagramPacket(m, m.length, aHost, 5600);
                        aSocket.send(request);

                        int len = 0;
                        byte[] buf = new byte[1024];

                        byte[] buffer = new byte[1000];
                        DatagramPacket reply = new DatagramPacket(buffer, buffer.length);

                        aSocket.setSoTimeout(2000);

                        while ((len = in.read(buf)) != -1) {
                            aSocket.receive(reply);
                            request = new DatagramPacket(buf, len, aHost, 5600);
                            aSocket.send(request);
                        }

                        aSocket.receive(reply);
                        in.close();
                        q.queueDequeue();
                    }
                }
            }
        } catch (SocketException e) {
            System.out.println("Socket: " + e.getMessage());
        } catch (IOException e) {
            //System.out.println("O outro servidor está fechado." + e.getMessage());
            try {
                Thread.sleep(5000);
            } catch (InterruptedException ex) {
                Thread.currentThread().interrupt();
            }
            enviaFile(q);
        }
    }

    public static void main(String args[]) {

        boolean check = startUp(); // processo principal pings de teste (5)

        Queue q = new Queue(20);
        Semaphore semaphorePass = new Semaphore(1);
        Semaphore semaphoreLog = new Semaphore(1);

        if (check) { // ja existe um servidor a correr, este servidor vai iniciar como backup
            System.out.println("Vou iniciar como secundário");

            new Thread() {
                public void run() {
                    try (DatagramSocket aSocket = new DatagramSocket(5500)) {
                        while (true) {
                            byte[] buffer = new byte[1000];
                            DatagramPacket request = new DatagramPacket(buffer, buffer.length);
                            aSocket.receive(request);
                            String s = new String(request.getData(), 0, request.getLength());

                            String[] aux = s.split(" ");

                            if (aux[0].contains("/MainServer"))
                                aux[0] = aux[0].replace("/MainServer", "/BackupServer");
                            else if (aux[0].contains("/BackupServer"))
                                aux[0] = aux[0].replace("/BackupServer", "/MainServer");

                            OutputStream out = new FileOutputStream(aux[0]);

                            int size = Integer.parseInt(aux[1]);

                            String strReply = "ola";
                            byte[] byteReply = strReply.getBytes();
                            InetAddress aHost = InetAddress.getByName("localhost");
                            DatagramPacket reply = new DatagramPacket(byteReply, byteReply.length, aHost, 5600);
                            aSocket.send(reply);

                            for (int i = 0; i < size; i++) {
                                byte[] buf = new byte[1024];
                                request = new DatagramPacket(buf, buf.length);
                                aSocket.receive(request);
                                out.write(request.getData(), 0, request.getLength());
                                aSocket.send(reply);
                            }

                            String[] pathSplitted  = aux[0].split("/");
                            System.out.println("Ficheiro recebido: "+ pathSplitted[pathSplitted.length-1]);

                            out.close();
                        }
                    } catch (SocketException e) {
                        System.out.println("Socket: " + e.getMessage());
                    } catch (IOException e) {
                        System.out.println("IO: " + e.getMessage());
                    }
                }
            }.start();

            heartbeat(); // processo principal envia pings periodicos para o servidor que esta a correr
        }

        recebeHeartbeat(); // inicia a thread para receber o heartbeat do servidor secundario

        System.out.println("Estou a iniciar como principal");
        // criar thread para receber pings periodicos

        readData();

        new Thread() {
            public void run() {
                enviaFile(q);
            }
        }.start();

        int numero = 0;

        try (ServerSocket listenSocket = new ServerSocket(serverPort)) {
            // System.out.println("A escuta no porto 6000");
            System.out.println("LISTEN SOCKET=" + listenSocket);
            while (true) {
                Socket clientSocket = listenSocket.accept(); // BLOQUEANTE
                System.out.println("CLIENT_SOCKET (created at accept())=" + clientSocket);
                numero++;
                new Connection(clientSocket, numero, q, semaphorePass, semaphoreLog);
            }
        } catch (IOException e) {
            System.out.println("Listen:" + e.getMessage());
        }
    }
}

// = Thread para tratar de cada canal de comunicação com um cliente
class Connection extends Thread {
    DataInputStream in;
    DataOutputStream out;
    Socket clientSocket;
    int thread_number;
    Queue q;
    Semaphore semaphorePass, semaphoreLog;

    public Connection(Socket aClientSocket, int numero, Queue qAux, Semaphore semaphorePassAux, Semaphore semaphoreLogAux) {
        thread_number = numero;
        q = qAux;
        semaphorePass = semaphorePassAux;
        semaphoreLog = semaphoreLogAux;
        try {
            clientSocket = aClientSocket;
            in = new DataInputStream(clientSocket.getInputStream());
            out = new DataOutputStream(clientSocket.getOutputStream());
            this.start();
        } catch (IOException e) {
            System.out.println("Connection:" + e.getMessage());
        }
    }


    public void updatePass(String user, String pass) {

        try {
            
            semaphorePass.acquire();
            
            String path = System.getProperty("user.dir") + "/data.txt";
            Scanner sc = new Scanner(new File(path));
            StringBuffer buffer = new StringBuffer();

            int counter = 0;

            String line = "", toReplace = "", newLine = "";

            while (sc.hasNextLine()) {
                line = sc.nextLine();
                if (counter % 3 == 0) {
                    if (line.indexOf(user) == 0)
                        toReplace = line;
                }

                buffer.append(line + System.lineSeparator());
                counter++;
            }
            String fileContents = buffer.toString();
            sc.close();

            newLine = user + ":" + pass;
            fileContents = fileContents.replaceAll(toReplace, newLine);
            FileWriter writer = new FileWriter(path);
            writer.append(fileContents);
            writer.flush();
            writer.close();

        } catch (IOException e1) {
            System.out.format("exception: ", e1);
        } catch (InterruptedException e2) {
            System.out.format("exception: ", e2);
        } finally {
            semaphorePass.release();
        }
    }

    public void updateLastDir(String user, String dir) {

        try {
            
            semaphoreLog.acquire();

            String path = System.getProperty("user.dir") + "/log.txt";
            Scanner sc = new Scanner(new File(path));
            StringBuffer buffer = new StringBuffer();

            String line = "", toReplace = "", newLine = "";

            while (sc.hasNextLine()) {
                line = sc.nextLine();
                if (line.indexOf(user) == 0) {
                    // System.out.println(line);
                    toReplace = line;
                }
                buffer.append(line + System.lineSeparator());
            }
            String fileContents = buffer.toString();
            sc.close();

            newLine = user + ":" + dir;
            fileContents = fileContents.replaceAll(toReplace, newLine);
            FileWriter writer = new FileWriter(path);
            writer.append(fileContents);
            writer.flush();
            writer.close();

        } catch (IOException e1) {
            System.out.format("exception: ", e1);
        } catch (InterruptedException e2) {
            System.out.format("exception: ", e2);
        } finally {
            semaphoreLog.release();
        }
    }

    private static String checkLastDir(String user) {

        String currentpath = System.getProperty("user.dir");
        File log = new File(currentpath + "/log.txt");

        String line;

        try (BufferedReader br = new BufferedReader(new FileReader(log))) {
            while ((line = br.readLine()) != null) {
                String[] splitted = line.split(":");
                if (splitted[0].equals(user)) {
                    if (splitted[1].equals("--"))
                        return currentpath + "/" + user;
                    else
                        return (splitted[1].replace("/MainServer", currentpath));
                }
            }
        } catch (IOException ex) {
            System.out.format("I/O exception: ", ex);
            return currentpath + "/" + user;
        }
        return currentpath + "/" + user;
    }

    private static String addDepth(int depth) {
        String s = "";
        for (int i = 0; i < depth; i++)
            s = s + " ";
        return s;
    }

    private static File checkFile(File curDir, String filename) {

        File[] filesList = curDir.listFiles();

        for (File f : filesList) {
            if (f.getName().equals(filename)) {
                if (f.isFile())
                    return f;
            }
        }

        return null;
    }

    private static String getFiles(String mode, int depth, File curDir, String lista) {

        File[] filesList = curDir.listFiles();

        for (File f : filesList) {
            if (mode.equals("-all")) {
                if (f.isDirectory()) {
                    lista = lista + addDepth(depth) + "/" + f.getName() + "\n";
                    lista = getFiles(mode, depth + 4, f, lista);
                }
            } else if (f.isDirectory())
                lista = lista + addDepth(depth) + "/" + f.getName() + "\n";
            if (f.isFile())
                lista = lista + addDepth(depth) + f.getName() + "\n";
        }

        return lista;
    }

    private static String findUserPass(String user, String pass) {
        String currentpath = System.getProperty("user.dir");
        File db = new File(currentpath + "/data.txt");

        String line;
        int counter = 0;

        try (BufferedReader br = new BufferedReader(new FileReader(db))) {
            while ((line = br.readLine()) != null) {
                if (counter % 3 == 0) {
                    String[] splitted = line.split(":");
                    if (user.equals(splitted[0]) && !pass.equals(splitted[1]))
                        return "Password invalida para este username!";
                    if (user.equals(splitted[0]) && pass.equals(splitted[1]))
                        return "Login efetuado com sucesso!";
                }
                counter++;
            }
        } catch (IOException ex) {
            System.out.format("I/O exception: ", ex);
        }
        return "Username nao exsite!";
    }

    // =============================

    public void run() {

        String resposta = "";
        String login[] = { "", "" };

        boolean logged = false;

        try {

            while (true) {
                while (logged == false) {
                    String loginInput = in.readUTF();
                    login = loginInput.split(" ");

                    if (login.length != 2) {
                        out.writeUTF("Input inválido!");
                    } else {
                        resposta = findUserPass(login[0], login[1]);
                        out.writeUTF(resposta);
                        if (resposta.equals("Login efetuado com sucesso!"))
                            logged = true;
                    }
                }

                String path = System.getProperty("user.dir");
                path = path + "/" + login[0];

                // verificar o ultimo path do ultimo login
                String currentpath = checkLastDir(login[0]);
                // update da ultima diretoria a ser consultada
                updateLastDir(login[0], currentpath.replace(System.getProperty("user.dir"), "/MainServer"));

                File dir = new File(currentpath);

                String data = in.readUTF();
                System.out.println("T[" + thread_number + "]: " + data);

                // -----------------------------------

                if (data.equals("sls")) {
                    resposta = getFiles("dir", 0, dir, "");
                    resposta = resposta.substring(0, resposta.length() - 1);
                    out.writeUTF(resposta);
                }

                else if (data.equals("sls -all")) {
                    resposta = getFiles("-all", 0, dir, "");
                    resposta = resposta.substring(0, resposta.length() - 1);
                    out.writeUTF(resposta);
                }

                else if (data.indexOf("ps -set") == 0) {

                    String comando[] = data.split(" ");

                    if (comando.length != 3)
                        out.writeUTF("Comando inválido!");
                    else {
                        updatePass(login[0], comando[2]);
                        out.writeUTF("Password atualizada!");
                        synchronized (q) { // synchronized block
                            q.queueEnqueue(System.getProperty("user.dir") + "/data.txt");
                        }
                        logged = false;
                    }
                }

                else if (data.indexOf("scd") == 0) {

                    if (data.equals("scd")) {
                        out.writeUTF(currentpath.replace(System.getProperty("user.dir"), "/MainServer"));
                    }

                    String cd[] = data.split(" ");
                    String cdAux;

                    if (cd.length == 2) {
                        if (!cd[0].equals("scd"))
                            out.writeUTF("Comando inválido!");

                        else {
                            cdAux = currentpath + "/" + cd[1];
                            File dirAux = new File(cdAux);

                            if (cd[1].equals(".")) {
                                currentpath = path;
                                out.writeUTF(currentpath.replace(System.getProperty("user.dir"), "/MainServer"));
                                updateLastDir(login[0],
                                        currentpath.replace(System.getProperty("user.dir"), "/MainServer"));
                            } else if (dirAux.isDirectory()) {
                                currentpath = cdAux;
                                out.writeUTF(currentpath.replace(System.getProperty("user.dir"), "/MainServer"));
                                updateLastDir(login[0],
                                        currentpath.replace(System.getProperty("user.dir"), "/MainServer"));
                            } else {
                                out.writeUTF("Diretoria inválida!");
                            }
                        }
                    }

                } else if (data.indexOf("getfile") == 0) {
                    String str[] = data.split(" ");
                    if (str.length != 2)
                        out.writeUTF("Comando inválido!");
                    else if (checkFile(dir, str[1]) == null)
                        out.writeUTF("Ficheiro não encontrado!");
                    else {

                        try (ServerSocket listenSocket2 = new ServerSocket(0)) {
                            out.writeUTF("A transferir ficheiro-" + Integer.toString(listenSocket2.getLocalPort()));
                            Socket fileSocket = listenSocket2.accept();
                            System.out.println("PORT " + Integer.toString(listenSocket2.getLocalPort()) + ": " + str[1]
                                    + " -> T[" + thread_number + "]");
                            new FilesPipe(fileSocket, "get", currentpath, str[1]);
                            listenSocket2.close();
                        } catch (IOException e) {
                            System.out.println("Listen:" + e.getMessage());
                        }
                    }

                } else if (data.indexOf("sendfile") == 0) {

                    try (ServerSocket listenSocket2 = new ServerSocket(0)) {
                        String str[] = data.split(" ");
                        out.writeUTF(path + " " + currentpath + " " + Integer.toString(listenSocket2.getLocalPort()));
                        Socket fileSocket = listenSocket2.accept();
                        System.out.println("PORT " + Integer.toString(listenSocket2.getLocalPort()) + ": " + str[1]
                                + " <- T[" + thread_number + "]");
                        new FilesPipe(fileSocket, "send", currentpath, str[1]);

                        synchronized (q) { // synchronized block
                            q.queueEnqueue(currentpath + "/" + str[1]);
                        }

                        //q.queueDisplay();

                        listenSocket2.close();
                    } catch (IOException e) {
                        System.out.println("Listen:" + e.getMessage());
                    }

                }

                else
                    out.writeUTF("Comando inválido!");

            }

        } catch (EOFException e) {
            System.out.println("EOF:" + e);
        } catch (IOException e) {
            System.out.println("IO:" + e);
        }
    }
}

class FilesPipe extends Thread {
    InputStream in;
    OutputStream out;
    String mode, file;
    Socket clientSocket;

    public FilesPipe(Socket aClientSocket, String mode, String current, String file) {
        try {
            clientSocket = aClientSocket;
            this.mode = mode;
            this.file = file;

            if (mode.equals("get")) {
                this.in = new FileInputStream(current + "/" + file);
                this.out = clientSocket.getOutputStream();
            } else {
                this.in = clientSocket.getInputStream();
                this.out = new FileOutputStream(current + "/" + file);
            }
            this.start();
        } catch (IOException e) {
            System.out.println("Connection:" + e.getMessage());
        }
    }

    public void run() {

        try {
            byte[] buf = new byte[1024];
            int len = 0;

            while ((len = in.read(buf)) != -1) {
                out.write(buf, 0, len);
            }

            out.close();
            in.close();

        } catch (EOFException e) {
            System.out.println("EOF:" + e);
        } catch (IOException e) {
            System.out.println("IO:" + e);
        }
    }
}
