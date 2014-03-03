import java.util.ArrayList;
import java.util.List;

public class KnightMoves {
    
    private int nrOfSequences = 0;
    private int keySeqLength;
    
    public static void main(String[] args) {
        KnightMoves km = new KnightMoves(10);
        km.getTotalSequencesForMoves();
        
    }
    
    public KnightMoves(int seqLength) {
        this.keySeqLength = seqLength;
        buildKeypad();
    }
    
    private Point[][] keyPad = new Point[4][5];

    private void buildKeypad() {
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
    
    private boolean isVowel(char v) {
    	if(v == 'A' || v =='E' || v == 'I' || v == 'O') {
    		return true;
    	}
    	
    	return false;
    }
    
    private List<Point> getKnightMoves(Point p) {
        int i = p.getRow();
        int j = p.getCol();
        List<Point> pts = new ArrayList<Point>();
        
        if( (i+2) < 4 && (j+1) < 5 && isNotNullPoint(i+2,j+1)) {
            pts.add(keyPad[i+2][j+1]);
            
        }
        
        if ( (i+2) < 4 && (j-1) >= 0 && isNotNullPoint(i+2,j-1)) {
            pts.add(keyPad[i+2][j-1]);
            
        } 

        if( (i+1) < 4 && (j+2) < 5  && isNotNullPoint(i+1,j+2)) {
            pts.add(keyPad[i+1][j+2]);
            
        }
        
        if ( (i+1) < 4 && (j-2) >= 0  && isNotNullPoint(i+1,j-2)) {
            pts.add(keyPad[i+1][j-2]);
            
        } 

        
        if (( (i-2) >= 0) && ((j+1) < 5 )  && isNotNullPoint(i-2,j+1)) {
            pts.add(keyPad[i-2][j+1]);
            
        } 
        
        if (( (i-2) >= 0) && ((j-1) >= 0 ) && isNotNullPoint(i-2,j-1)) {
            pts.add(keyPad[i-2][j-1]);
            
        } 

        if (( (i-1) >= 0) && ((j-2) >= 0) && isNotNullPoint(i-1,j-2)) {
            pts.add(keyPad[i-1][j-2]);
            
        } 

        if (( (i-1) >= 0) && ((j+2) < 5) && isNotNullPoint(i-1,j+2)) {
            pts.add(keyPad[i-1][j+2]);
            
        } 

        return pts;
    }
    
    private boolean isNotNullPoint(int r, int c) {
    	if(keyPad[r][c] != null) {
    		return true;	
    	} else {
    		//System.out.println("Hit a null key at: " + r + "," + c);
    		return false;
    	}
    	
        
    }
    
    public void getTotalSequencesForMoves() {
        
    	int totalSequences = 0;
        //All starting points on the keypad
        for(int i=0;i<4;i++ ) {
            for(int j=0;j<5; j++) {
                //Skip the empty cells in the grid
                if( i==3 && (j==0 || j==4) ) {
                    continue;
                }
                
                if(isVowel(keyPad[i][j].getKey())) {
                	countMoves(keySeqLength,1, keyPad[i][j]);
                	
                } else {
                	countMoves(keySeqLength,2, keyPad[i][j]);
                	
                }
                
                System.out.println("Done for Point: " + i + "," + j + ". Count: " + nrOfSequences);
                totalSequences += nrOfSequences;
                nrOfSequences=0;
            }
        }
        
        System.out.println("Total Number of sequences: " + totalSequences);
    }
    
    public void countMoves(int movesLeft, int vowelsLeft, Point pt) {
    	
    	
    	if (movesLeft > 0) {
    		
    		List<Point> newPoints = getKnightMoves(pt);
    		
    		if(movesLeft ==1) {
    			nrOfSequences += newPoints.size();
    		}
    		
    		for(Point p: newPoints) {
    			
    			if(isVowel(p.getKey())) {
    				
    				if (vowelsLeft >0) {
    					countMoves(movesLeft-1,vowelsLeft-1,p);
    					
    				} else {
    		    		if(movesLeft ==1) {
    		    			nrOfSequences--;
    		    		}
    		    		
    				}
    				
    			} else {
    				countMoves(movesLeft-1,vowelsLeft,p);
    				
    			}
    		}
    		
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
        
    }
}
