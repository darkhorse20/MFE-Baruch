package com.jpm.casestudy;

import java.io.BufferedReader;
import java.io.FileReader;
import java.text.DateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.StringTokenizer;

/**
 * @author Megha
 *
 */
public class EconomicVariables {
	private ArrayList<Date> dates = new ArrayList<Date>();
	private ArrayList<Double> libor = new ArrayList<Double>();
	private ArrayList<Double> hpiNy = new ArrayList<Double>();
	private ArrayList<Double> hpiCa = new ArrayList<Double>();
	private ArrayList<Double> mtgRates = new ArrayList<Double>();
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
					dates.add(Utils.formatter.parse(t.nextToken()));
					libor.add(Double.parseDouble(t.nextToken()));
					hpiNy.add(Double.parseDouble(t.nextToken()));
					hpiCa.add(Double.parseDouble(t.nextToken()));
					mtgRates.add(Double.parseDouble(t.nextToken()));
				}
				line = b.readLine();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		
	}

	public ArrayList<Date> getDates() {
		return dates;
	}

	public void setDates(ArrayList<Date> dates) {
		this.dates = dates;
	}

	public ArrayList<Double> getLibor() {
		return libor;
	}

	public void setLibor(ArrayList<Double> libor) {
		this.libor = libor;
	}

	public ArrayList<Double> getHpiNy() {
		return hpiNy;
	}

	public void setHpiNy(ArrayList<Double> hpiNy) {
		this.hpiNy = hpiNy;
	}

	public ArrayList<Double> getHpiCa() {
		return hpiCa;
	}

	public void setHpiCa(ArrayList<Double> hpiCa) {
		this.hpiCa = hpiCa;
	}

	public ArrayList<Double> getMtgRates() {
		return mtgRates;
	}

	public void setMtgRates(ArrayList<Double> mtgRates) {
		this.mtgRates = mtgRates;
	}

	public String getFileConf() {
		return fileConf;
	}

	public void setFileConf(String fileConf) {
		this.fileConf = fileConf;
	}
	
	
}
