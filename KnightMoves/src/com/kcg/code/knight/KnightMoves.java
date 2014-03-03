package com.kcg.code.knight;

import java.util.ArrayList;
import java.util.List;

/**
 * @author Anuroop
 *
 */
public class KnightMoves {
	
	private int keyNr;
	private int keySeqLength;
	
	public static void main(String[] args) {
		
	}
	
	public KnightMoves(int seqLength) {
		this.keySeqLength = seqLength;
		loadKeypad();
	}
	
	private Point[][] keyPad;

	private void loadKeypad() {
		keyPad[0][0] = new Point(0,0, 'A');
		keyPad[0][1] = new Point(0,1, 'B');
		keyPad[0][2] = new Point(0,2, 'C');
		keyPad[0][3] = new Point(0,3, 'D');
		keyPad[0][4] = new Point(0,4, 'E');
		keyPad[1][0] = new Point(1,0, 'F');
		keyPad[1][1] = new Point(1,1, 'G');
		keyPad[1][2] = new Point(1,2, 'H');
		keyPad[1][3] = new Point(1,3, 'I');
		keyPad[1][4] = new Point(1,4, 'J');
		keyPad[2][0] = new Point(2,0, 'K');
		keyPad[2][1] = new Point(2,1, 'L');
		keyPad[2][2] = new Point(2,2, 'M');
		keyPad[2][3] = new Point(2,3, 'N');
		keyPad[2][4] = new Point(2,4, 'O');
		keyPad[3][0] = null;
		keyPad[3][1] = new Point(3,1, '1');
		keyPad[3][2] = new Point(3,2, '2');
		keyPad[3][3] = new Point(3,3, '3');
		keyPad[3][4] = null;
		
	}
	
	private Point[] getKnightMoves(Point p) {
		int i = p.getRow();
		int j = p.getCol();
		List<Point> pts = new ArrayList<Point>();
		
		if(( (i+2) < 4) && (j+1) < 5 ){
			pts.add(keyPad[i+2][j+1]);
			
		} else if( ( (j+2) < 5) && ((i+1) < 4 ) ) {
			pts.add(keyPad[i+2][j+1]);
			
		} else if ( ( (j+2) < 5) && ((i+1) < 4 ) ) {
			pts.add(keyPad[i+2][j+1]);
			
		}
		
		return (Point[])pts.toArray();
	}
	
	
	public void getTotalSequencesForMoves(int nrMoves, int i, int j) {
		
		Point p = keyPad[i][j];
		Point[] pts = getKnightMoves(p);
		while(pts.length > 0 && keySeqLength <=10) {
			keySeqLength++;
			
		}
	}
	
	private class Point {
		private int i,j;
		private char key;
		
		public Point(int row, int col, char c) {
			this.i = row;
			this.j = col;
			this.key = c;
			
		}
		
		public int getRow() {
			return this.i;
		}
		
		public int getCol() {
			return this.j;
			
		}

		public char getKey() {
			return key;
		}

		public void setKey(char key) {
			this.key = key;
		}
		
		
	}
}
