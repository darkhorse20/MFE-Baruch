package com.jpm.casestudy;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.StringTokenizer;

/**
 * @author Megha
 *
 */
public class EconomicVariables {
	private HashMap<Date, VariableRecord> variableMap = new HashMap<Date, EconomicVariables.VariableRecord>();
	private HashMap<String, HashMap<Date, Double>> hpiMap = new HashMap<String, HashMap<Date, Double>>();
	private String fileConf;
	
	public EconomicVariables(String file) {
		this.fileConf = file;
		
		load();
	}
	
	private void load() {
		try {
			
			FileReader r = new FileReader(fileConf);
			BufferedReader b = new BufferedReader(r);
			String line = b.readLine();
			while(line !=null) {
				StringTokenizer t = new StringTokenizer(line,",");
				if(t.countTokens() > 0) {
				    Date key = Utils.formatter.parse(t.nextToken());
				    Double libor = Double.parseDouble(t.nextToken());
				    Double hpiNY = Double.parseDouble(t.nextToken());
				    Double hpiCA = Double.parseDouble(t.nextToken());
				    Double mtgRate = Double.parseDouble(t.nextToken());
				    
				    variableMap.put(key,
				            new VariableRecord(key,libor, mtgRate)
				    );
				    
				    
				    if(hpiMap.containsKey("NY")) {
				    	hpiMap.get("NY").put(key, hpiNY);
				    	
				    	
				    } else {
				    	HashMap<Date, Double> val = new HashMap<Date, Double>();
				    	val.put(key, hpiNY);
				    	hpiMap.put("NY", val);
					    
				    }
				    
				    if(hpiMap.containsKey("CA")) {
				    	hpiMap.get("CA").put(key, hpiCA);
				    	
				    	
				    } else {
				    	HashMap<Date, Double> val = new HashMap<Date, Double>();
				    	val.put(key, hpiCA);
				    	hpiMap.put("CA", val);
					    
				    }

				}
				line = b.readLine();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}


	}

    
    public Double getLibor(Date date) {
        return this.variableMap.get(date).libor;
    }
    
    public Double getMtgRate(Date date) {
        return this.variableMap.get(date).mtgRate;
    }
    
    public Double getHPI(Date dt, String state) {
    	return this.hpiMap.get(state).get(dt);
    }
    
    
	   private class VariableRecord {
	        
	        private Double libor;
	        private Double mtgRate;
	        private Date date;
	        
	        public VariableRecord(Date dt, Double lib, Double mRate) {
	            this.libor = lib;
	            this.mtgRate = mRate;
	            this.date = dt;
	        }

	        
	    }

	
	
}
