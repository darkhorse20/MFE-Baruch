package com.jpm.casestudy;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

import com.jpm.casestudy.AmortizationSchedule.Record;



public class Utils {
	public static SimpleDateFormat formatter = new SimpleDateFormat("MM/dd/yyyy");
    
    public static void write(List<Record> schedule, String path) {
        File f = new File(path);
        
        try {
            f.createNewFile();
      
            FileWriter fw = new FileWriter(f);    
            for(Record r: schedule) {
                fw.write(r.getDate() + "\t" + r.getBalance()+ "\t" + r.getPrincipal() + "\t" + r.getInterest()  + "\n");
                
            }
            fw.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
            
        }
        
    }
    
    public static void writeBooks(List<List<Integer>> books, String path) {
        File f = new File(path);
        
        try {
            f.createNewFile();
      
            FileWriter fw = new FileWriter(f); 
            for(int i=0; i < books.size(); i++) {
                   for(int j=0; j< books.get(i).size(); j++) {
                       fw.write(books.get(i).get(j)+ ",");       
                   }
                   fw.write("\n");
                
            }
            fw.flush();
        } catch (IOException e) {
            throw new RuntimeException(e);
            
        }
        
    }
    
    public static void aggregateLists(List<Integer> to, List<Integer> from) {
        if(from == null) {
            return;
        }
        for(int i=0; i<from.size(); i++) {
            if(to.size() > i) {
                to.set(i, (from.get(i) + to.get(i))/2); 
            } else {
                to.add(i, from.get(i));
            }
            
        }
    }
    
    public static void aggregateListsDouble(List<Double> to, List<Double> from) {
        for(int i=0; i<from.size(); i++) {
            if(to.size() > i) {
                to.set(i, (from.get(i) + to.get(i))/2); 
            } else {
                to.add(i, from.get(i));
            }
            
        }
    }
    
    public static void addBook(List<List<Integer>> to, List<Integer> from) {
        to.add(from);
    }
    

    
    public static void calcDiffusionConstants(List<Double> aveMidPrxs, List<Double> diffs) {
    	int l = 0; //The start time t
    	for(int i=1; i < aveMidPrxs.size(); i++) {
    		diffs.add(((aveMidPrxs.get(l) - aveMidPrxs.get(l+i))*(aveMidPrxs.get(l) - aveMidPrxs.get(l+i)))/i); 
    		
    	}
    	
    }
}
