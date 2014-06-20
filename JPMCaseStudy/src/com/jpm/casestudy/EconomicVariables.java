package com.jpm.casestudy;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Date;
import java.util.HashMap;
import java.util.StringTokenizer;

/**
 * @author Megha
 *
 */
public class EconomicVariables {
	private HashMap<Date, VariableRecord> variableMap = new HashMap<Date, EconomicVariables.VariableRecord>(); 
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
				    variableMap.put(key,
				            new VariableRecord(key, 
				                    Double.parseDouble(t.nextToken()), 
				                    Double.parseDouble(t.nextToken()),
				                    Double.parseDouble(t.nextToken()), 
				                    Double.parseDouble(t.nextToken())
				                      )
				    );
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
    
    
    
	   private class VariableRecord {
	        
	        private Double libor;
	        private Double hpiNy;
	        private Double hpiCa;
	        private Double mtgRate;
	        private Date date;
	        
	        public VariableRecord(Date dt, Double lib, Double hpiNy, Double hpiCa, Double mRate) {
	            this.libor = lib;
	            this.hpiCa = hpiCa;
	            this.hpiNy = hpiNy;
	            this.mtgRate = mRate;
	            this.date = dt;
	        }

	        
	    }

	
	
}
