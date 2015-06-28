/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package test_for_c_server;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 *
 * @author Remco
 */
public class Test_for_c_server {

    /**
     * http://stackoverflow.com/questions/19561941/socket-comunication-java-client-c-server
     * @param args 
     */
    public static void main(String[] args) {
        
        try {        
            String hostName = "192.168.1.200"; // pi: 192.168.1.102, 
            int portNumber = 1000;
            Socket firstSocket = new Socket(hostName, portNumber);
            PrintWriter out = new PrintWriter(firstSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(firstSocket.getInputStream()));
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
            String userInput;
            String input;
            
            Thread thread = new Thread() {
                public void run() {
                    String input;
                    try {
                        while((input = in.readLine()) != null) {
                            System.out.println("Server says: " + input);
                        }    
                    }
                    catch(IOException ex) {
                        
                    }                    
                }                
            };thread.start();
            
            while ((input = stdIn.readLine()) != null) 
            {                
                System.out.println("You said: " + input);
                out.println(input);
                if(input.equals("SPAM")) {
                    for(int i = 0; i<9999; i++) {
                        out.println("SPAM!!!" + i);
                    }
                    System.out.println("Spam don.");
                }
            }
            in.close();
            stdIn.close();
            firstSocket.close();
        }
        catch(IOException e) {
            
        }

    }
    
}
