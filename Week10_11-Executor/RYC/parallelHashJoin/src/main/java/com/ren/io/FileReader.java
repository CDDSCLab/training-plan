package com.ren.io;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;

public class FileReader {

    private String path = null;

    private java.io.FileReader fr = null;
    private BufferedReader br = null;

    public FileReader(String path) {
        this.path = path;
        try {
            fr = new java.io.FileReader(this.path);
            br = new BufferedReader(fr);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    public String readLine() {
        try {
            return br.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public void close(){
        try {
            br.close();
            fr.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
