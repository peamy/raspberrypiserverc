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
            String hostName = "127.0.0.1";
            int portNumber = 5000;
            Socket firstSocket = new Socket(hostName, portNumber);
            PrintWriter out = new PrintWriter(firstSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(firstSocket.getInputStream()));
            BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
            String userInput;
            String input;
            
            while ((input = in.readLine()) != null) 
            {
                System.out.println("Server says: " + input); 
                userInput = stdIn.readLine();
                out.println(userInput);  
            }
            in.close();
            stdIn.close();
            firstSocket.close();
        }
        catch(IOException e) {
            
        }

    }
    
}
