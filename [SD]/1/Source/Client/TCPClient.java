//package TP1.Clients;
//package sockets;

import java.net.*;
import java.util.Scanner;
import java.io.*; 

public class TCPClient extends Thread {
	private int serversocket;
	private String ip;
	private int serversocket2;
	private String ip2;

	public TCPClient() {}
	
	public void setServersocket(int serversocketAux) {
		this.serversocket = serversocketAux;
	}

	public int getServersocket() {
		return this.serversocket;
	}

	public void setIp(String ipAux) {
		this.ip = ipAux;
	}

	public String getIp() {
		return this.ip;
	}

	public void setServersocket2(int serversocket2Aux) {
		this.serversocket2 = serversocket2Aux;
	}

	public int getServersocket2() {
		return this.serversocket2;
	}
	
	public void setIp2(String ip2Aux) {
		this.ip2 = ip2Aux;
	}

	public String getIp2() {
		return this.ip2;
	}

	private static String addDepth(int depth) {
        String s = "";
        for (int i = 0; i<depth;i++) s = s + " ";
        return s;
    }

	private void readConfig() {

        //leitura do ficheiro database
        String currentpath = System.getProperty("user.dir");
        File db = new File(currentpath + "/config.txt");

		System.out.println(currentpath);
        
        String line;
        int counter = 0;

        try ( BufferedReader br = new BufferedReader(new FileReader(db))) {
            while ((line = br.readLine()) != null) {
                if (counter == 0) setServersocket(Integer.parseInt(line));
				if (counter == 1) setIp(line);
				if (counter == 2) setServersocket2(Integer.parseInt(line));
				if (counter == 3) setIp2(line);
                counter++;
            } 
        } catch (IOException ex) {
            System.out.format("I/O exception: ", ex);
        }
    }

	public static void updateServer1(String ipAddr, int porto) {

        try {
            String path = System.getProperty("user.dir")+"/config.txt";
            Scanner sc = new Scanner(new File(path));
            StringBuffer buffer = new StringBuffer();

			String line = "";
			int counter = 0;

            while (sc.hasNextLine()) {
                line = sc.nextLine();
				if (counter == 0) line = String.valueOf(porto);
				if (counter == 1) line = ipAddr;
				counter++;
                buffer.append(line+System.lineSeparator());
            }
            String fileContents = buffer.toString();
            sc.close();

            FileWriter writer = new FileWriter(path);
            writer.append(fileContents);
            writer.flush();  
            writer.close();

        } catch (IOException e1) {
            System.out.format("exception: ", e1);
        }
    }

	public static void updateServer2(String ipAddr, int porto) {

        try {
            String path = System.getProperty("user.dir")+"/config.txt";
            Scanner sc = new Scanner(new File(path));
            StringBuffer buffer = new StringBuffer();

			String line = "";
			int counter = 0;

            while (sc.hasNextLine()) {
                line = sc.nextLine();
				if (counter == 2) line = String.valueOf(porto);
				if (counter == 3) line = ipAddr;
				counter++;
                buffer.append(line+System.lineSeparator());
            }
            String fileContents = buffer.toString();
            sc.close();

            FileWriter writer = new FileWriter(path);
            writer.append(fileContents);
            writer.flush();  
            writer.close();

        } catch (IOException e1) {
            System.out.format("exception: ", e1);
        }
    }

	private static File checkFile(File curDir, String filename) {

        File[] filesList = curDir.listFiles();

        for(File f : filesList){
            if (f.getName().equals(filename)) {
				if (f.isFile()) return f;
			}
        }

        return null;
    }

    private static String getFiles(String mode, int depth, File curDir, String lista) {

        File[] filesList = curDir.listFiles();

        for(File f : filesList){
            if (mode.equals("-all")) {
                if(f.isDirectory()) {
                    lista=lista+addDepth(depth)+"/"+f.getName()+"\n";
                    lista = getFiles(mode,depth+4,f,lista);
                }
            }
            else if(f.isDirectory()) lista=lista+addDepth(depth)+"/"+f.getName()+"\n";
            if(f.isFile()) lista=lista+addDepth(depth)+f.getName()+"\n";
        }

        return lista;
    }

	private void cliente(Scanner sc,String serverDir, int failCount) {
		String path = System.getProperty("user.dir")+"/ficheiros";
		String currentpath = path;

		try (Socket s = new Socket(getIp(), getServersocket())) {
			failCount = 0; //reset do contador de fails

			System.out.println("SOCKET=" + s);

			s.setSoTimeout(5000);

			// 2o passo
			DataInputStream in = new DataInputStream(s.getInputStream());
			DataOutputStream out = new DataOutputStream(s.getOutputStream());

			// 3o passo
			//try (sc) {
				//login
			while (true) {
				System.out.print("Insira o nome de utilizador e a palavra-passe: ");
				String login = sc.nextLine();
				out.writeUTF(login);
				String resposta = in.readUTF();
				System.out.println(resposta);
				if (resposta.equals("Login efetuado com sucesso!")) 
					break;
			}

			while (true) {
				// READ STRING FROM KEYBOARD
				String texto = sc.nextLine();

				if (texto.indexOf("cd") == 0) {

					if (texto.equals("cd")) {
						System.out.println(currentpath.replace(System.getProperty("user.dir"), "/Client"));
					}
					
					String cdAux;
					String cd[] = texto.split(" ");

					if (cd.length == 2) {
						if (!cd[0].equals("cd")) System.out.println("Comando inválido!"); 
	
						else {
							cdAux = currentpath+"/"+cd[1];
							File dirAux = new File(cdAux);
	
							if (cd[1].equals(".")) {
								currentpath = path;
								System.out.println(currentpath.replace(System.getProperty("user.dir"), "/Client"));
							}
							else if (dirAux.isDirectory()) {
								currentpath = cdAux;
								System.out.println(currentpath.replace(System.getProperty("user.dir"), "/Client"));
							}
							else {
								System.out.println("Diretoria inválida!");
							}
						}
					}
				
				} else if (texto.indexOf("ps -set ") == 0) {
					out.writeUTF(texto);
					String data = in.readUTF();
					System.out.println(data);

					while (true) {
						System.out.print("Insira o nome de utilizador e a palavra-passe: ");
						String login = sc.nextLine();
						out.writeUTF(login);
						String resposta = in.readUTF();
						System.out.println(resposta);
						if (resposta.equals("Login efetuado com sucesso!")) 
							break;
					}		

				} else if (texto.equals("ls")) {
					File dir = new File(currentpath);
					System.out.print(getFiles("dir",0,dir,""));  
				
				} else if (texto.equals("ls -all")) {
					File dir = new File(currentpath);
					System.out.print(getFiles("-all",0,dir,""));

				} else if (texto.indexOf("main") == 0) {
					String str[] = texto.split(" ");

					if (texto.equals("main")) System.out.println(getIp()+ " " +getServersocket());

					else if (str.length == 4) {
						if (!str[0].equals("main") || !str[1].equals("-set")) System.out.println("Comando inválido!"); 
						else updateServer1(str[2],Integer.parseInt(str[3]));
					}
					else System.out.println("Comando inválido!");

				} else if (texto.indexOf("backup") == 0) {
					String str[] = texto.split(" ");

					if (texto.equals("backup")) System.out.println(getIp2()+" "+getServersocket2());

					else if (str.length == 4) {
						if (!str[0].equals("backup") || !str[1].equals("-set")) System.out.println("Comando inválido!"); 
						else updateServer2(str[2],Integer.parseInt(str[3]));
					}
					else System.out.println("Comando inválido!");
				
				} else if (texto.indexOf("getfile") == 0) {
					out.writeUTF(texto);
					String data = in.readUTF();
					
					if (!(data.indexOf("A transferir ficheiro") == 0)) {
						System.out.println(data);
					}

					if (data.indexOf("A transferir ficheiro") == 0) {
						String str[] = texto.split(" ");
						String aux[] = data.split("-");
						System.out.println(aux[0]+ " " + str[1] + " para " + currentpath.replace(System.getProperty("user.dir"), "/Client"));

						final String pathaux = currentpath;
						
						new Thread() {
							public void run() {
								try (Socket s2 = new Socket(getIp(),Integer.parseInt(aux[1]))){
									InputStream in2 = s2.getInputStream();
									OutputStream out2 = new FileOutputStream(pathaux+"/"+str[1]);

									byte[] buf = new byte[1024];
									int len = 0;
									
									while ((len = in2.read(buf)) != -1) {
										out2.write(buf, 0, len);
									}
									
									out2.close();
									in2.close();

									System.out.println("Ficheiro recebido: "+pathaux.replace(System.getProperty("user.dir"), "/Client")+"/"+str[1]);

									s2.close();

								} catch (UnknownHostException e) {
									System.out.println("Sock:" + e.getMessage());
								} catch (EOFException e) {
									System.out.println("EOF:" + e.getMessage());
								} catch (IOException e) {
									System.out.println("IO:" + e.getMessage());
								}
							}
						}.start();
					}

				} else if (texto.indexOf("sendfile") == 0) {
					String str[] = texto.split(" ");
					File dir = new File(currentpath);
					if (str.length != 2) System.out.println("Comando inválido!");
					else if (checkFile(dir,str[1]) == null) System.out.println("Ficheiro não encontrado!");
					else {
						out.writeUTF(texto);
						String aux[] = (in.readUTF()).split(" ");
						System.out.println("A enviar " + str[1] + " para " + aux[1].replace(aux[0], serverDir));
						final String pathaux = currentpath;
						
						new Thread() {
							public void run() {
								try (Socket s2 = new Socket(getIp(),Integer.parseInt(aux[2]))){
									InputStream in2 = new FileInputStream(pathaux+"/"+str[1]);
									OutputStream out2 = s2.getOutputStream();

									byte[] buf = new byte[1024];
									int len = 0;
									
									while ((len = in2.read(buf)) != -1) {
										out2.write(buf, 0, len);
									}
									
									out2.close();
									in2.close();

									System.out.println("Ficheiro enviado: "+aux[1].replace(aux[0], serverDir)+"/"+str[1]);

									s2.close();

								} catch (UnknownHostException e) {
									System.out.println("Sock:" + e.getMessage());
								} catch (EOFException e) {
									System.out.println("EOF:" + e.getMessage());
								} catch (IOException e) {
									System.out.println("IO:" + e.getMessage());
								}
							}
						}.start();
					}
			

				} else { 
					out.writeUTF(texto);
					String data = in.readUTF();
					System.out.println(data);
				}
			}
			
		} catch (UnknownHostException e) {
			System.out.println("Sock:" + e.getMessage());
		} catch (EOFException e) {
			System.out.println("EOF:" + e.getMessage());

			System.out.println("Servidor Indisponível! Trocando de servidor...");

			String ipAux = getIp();
			int portAux = getServersocket();
			setIp(getIp2());
			setServersocket(getServersocket2());
			setIp2(ipAux);
			setServersocket2(portAux);

			String dirAux = "";
			
			if (serverDir.equals("/MainServer")) dirAux = "/BackupServer";
			else dirAux = "/MainServer";

			failCount++;

			if (failCount < 2) cliente(sc, dirAux, failCount);
			else {
				System.out.println("Erro de conexão! Nenhum dos servidores está ligado.");
				return;
			}
			
		} catch (IOException e) {
			System.out.println("Servidor Indisponível! Trocando de servidor...");

			String ipAux = getIp();
			int portAux = getServersocket();
			setIp(getIp2());
			setServersocket(getServersocket2());
			setIp2(ipAux);
			setServersocket2(portAux);

			String dirAux = "";
			
			if (serverDir.equals("/MainServer")) dirAux = "/BackupServer";
			else dirAux = "/MainServer";

			failCount++;

			if (failCount < 2) {
				cliente(sc,dirAux, failCount);
			}
			else {
				System.out.println("Erro de conexão! Nenhum dos servidores está ligado.");
				return;
			}
		}
	}
	
	public static void main(String args[]) {
		TCPClient tcpclient = new TCPClient();		

		tcpclient.readConfig();

		try (Scanner sc = new Scanner(System.in)) {
			tcpclient.cliente(sc,"/MainServer", 0);
		}  catch (java.util.NoSuchElementException e) {
			System.out.println("No Such Element Exception:" + e.getMessage());
		}
	}
}