package com.jpm.casestudy;

import java.util.Date;

public class Deal {
	private Date origDate;
	private String state;
	private Double origAmt;
	private Double oLtv;
	private Double oFico;
	private Integer term;
	private Integer fixedTerm;
	private Double fixedRate;
	private String index;
	private Double balance;
	private Integer age;
	private Double offset;
	
	
	public Deal( Date date, String state, Double amt, Double ltv,
	 Double fico,
	 Integer term,
	 Integer fixTerm,
	 Double fixRate,
	 String indx,
	 Double bal,
	 Integer age,
	 Double offset
) {
		this.origDate = date;
		this.state = state;
		this.origAmt = amt;
		this.oLtv = ltv;
		this.oFico = fico;
		this.term = term;
		this.fixedTerm = fixTerm;
		this.fixedRate = fixRate;
		this.index = indx;
		this.balance = bal;
		this.age = age;
		this.offset = offset;
		
	}


	public Date getOrigDate() {
		return origDate;
	}


	public String getState() {
		return state;
	}


	public Double getOrigAmt() {
		return origAmt;
	}


	public Double getoLtv() {
		return oLtv;
	}


	public Double getoFico() {
		return oFico;
	}


	public Integer getTerm() {
		return term;
	}


	public Integer getFixedTerm() {
		return fixedTerm;
	}


	public Double getFixedRate() {
		return fixedRate;
	}


	public String getIndex() {
		return index;
	}


	public Double getBalance() {
		return balance;
	}


	public Integer getAge() {
		return age;
	}


	public Double getOffset() {
		return offset;
	}
	
	
}
