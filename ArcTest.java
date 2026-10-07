/*
 -PAG
 -13/10/08
 This is a polyrhythm generator for musicians.
 
 */

/*
 * @(#)PolyGene.java	1.10 03/01/23
 */

import java.awt.*;
import java.awt.event.*;
import java.applet.*;
import java.lang.*;

/**
 * An interactive test of the Graphics.drawArc and Graphics.fillArc
 * routines. Can be run either as a standalone application by
 * typing "java ArcTest" or as an applet in the AppletViewer.
 */
public class ArcTest extends java.applet.Applet implements Runnable
{
    ArcControls controls;   // The controls for marking and filling arcs
    ArcCanvas canvas;       // The drawing area to display arcs
	Thread animator;
	Thread animator2;
	int frame;
	int delay;

    public void init() 
	{
		setLayout(new BorderLayout());
		canvas = new ArcCanvas();
		add("Center", canvas);
		add("South", controls = new ArcControls(canvas));
		delay = 1000 / 30;
    }

    public void destroy() {
        remove(controls);
        remove(canvas);
    }

    public void start() 
	{
		controls.setEnabled(true);
		
		animator = new Thread(this);
	//	animator.start();
		animator2 = new Thread(this);
	//	animator2.start();
    }

    public void stop() {
	controls.setEnabled(false);
		animator = null;
		animator2 = null;
    }

    public void processEvent(AWTEvent e) {
        if (e.getID() == Event.WINDOW_DESTROY) {
            System.exit(0);
        }
    }

    public static void main(String args[]) {
	Frame f = new Frame("ArcTest");
	ArcTest	arcTest = new ArcTest();

	arcTest.init();
	arcTest.start();

	f.add("Center", arcTest);
	f.setSize(300, 300);
	f.show();
    }
	
	/**
     * This method is called by the thread that was created in
     * the start method. It does the main animation.
     */
    public void run() 
	{
		// Remember the starting time
		long tm = System.currentTimeMillis();
		while (Thread.currentThread() == animator) 
		{
			// Display the next frame of animation.
		//	canvas.setCotes(7);
			repaint();
		//	canvas.setCotes(frame);
		//	canvas.paint2(canvas.getGraphics(), frame);
		//	canvas.update(canvas.getGraphics());
		//	repaint();
			//canvas.redraw(false,5,360);
			// Advance the frame
			frame++;
			// Delay depending on how far we are behind.
			try {
			//	canvas.update(canvas.getGraphics());
				tm += delay;
				Thread.sleep(Math.max(0, tm - System.currentTimeMillis()));
			} catch (InterruptedException e) {
				break;
			}
			
		}
		while (Thread.currentThread() == animator2) 
		{
			// Display the next frame of animation.
			canvas.setCotes(7);
			repaint();
			//canvas.redraw(false,5,360);
			// Delay depending on how far we are behind.
			try {
		//		canvas.update(canvas.getGraphics());
				tm += delay;
				Thread.sleep(Math.max(0, tm - System.currentTimeMillis()));
			} catch (InterruptedException e) {
				break;
			}
			
			// Advance the frame
			frame++;
		}
	}
	
	public void paint(Graphics g) 
	{
		//canvas.paint2(canvas.getGraphics(), frame);
	//	canvas.setCotes(3);
	//	canvas.paint2(canvas.getGraphics(), frame);

    }
	
	///public void update(Graphics g)
//{
	//	canvas.initDraw(g);
	//	paint(canvas.getGraphics());
	//}
	
    public String getAppletInfo() {
        return "An interactive test of the Graphics.drawArc and \nGraphics.fillArc routines. Can be run \neither as a standalone application by typing 'java ArcTest' \nor as an applet in the AppletViewer.";
    }
}

class ArcCanvas extends Canvas implements Runnable
{
    int		startAngle = 0;
	int		onlyAngle = 0;
    int		endAngle = 360;
    boolean	clear = false;
    Font	font = new java.awt.Font("Courier", Font.BOLD, 14);
	boolean FirstTime = true;
	Thread Poly;
	int frame;
	int delay;
	
//	int tabFormes[] = {5,6,7,8};
//	int iNbForme = 0;
	
	public void init() 
	{
		delay = 1000 / 30;
    }
	public void start() 
	{
		Poly = new Thread(this);
	//	Poly.start();
    }
	
    public void stop() 
	{
		Poly = null;
    }
	
	public void run() 
	{
		// Remember the starting time
		long tm = System.currentTimeMillis();
		while (Thread.currentThread() == Poly) 
		{
			// Display the next frame of animation.
			repaint();
			//canvas.redraw(false,5,360);
			// Delay depending on how far we are behind.
			try {
			//	this.update(this.getGraphics());
				tm += delay;
				Thread.sleep(Math.max(0, tm - System.currentTimeMillis()));
			} catch (InterruptedException e) {
				break;
			}
			// Advance the frame
			frame++;
		}
	}
	
	public int getCotes()
	{
		return startAngle;
	}
	
	public void setCotes(int Cotes)
	{
		startAngle = Cotes;
	}
	
	public void resetCotes()
	{
		startAngle = onlyAngle;
	}
	
	public void paint(Graphics g) 
	{
		//canvas.paint2(canvas.getGraphics(), frame);
	//	this.setCotes(5);
		this.paint2(this.getGraphics(), 0);
    }

    public void paint2(Graphics g, int Surplus) 
	{
		//if(FirstTime)
	////	{
			initDraw(g);
	//		FirstTime = false;
	//	}
		Graphics2D g2 = (Graphics2D) g;
		
	//	BasicStroke stroke;
	//	stroke = new BasicStroke(5, 5, 0);
		
		Rectangle r = getBounds();
	
		switch(startAngle)
		{
			case 3:
				g2.setColor(Color.blue);
				break;
			case 4:
				g2.setColor(Color.red);
				break;
			case 5:
				g2.setColor(Color.green);
				break;
			case 6:
				g2.setColor(Color.orange);
				break;
			case 7:
				g2.setColor(Color.gray);
				break;
			case 8:
				g2.setColor(Color.black);
				break;
			default:
				g2.setColor(Color.blue);
				break;
		}
		
		double x = 0;
		double y = 0;
		int ioldx =  (r.width/2);
		int ioldy = 0;
		int ix;
		int iy;
		double oldx = (r.width/2);
		double oldy = 0;
		
		int iTabPremierPolyx[];
		int iTabPremierPolyy[];

		iTabPremierPolyx = new int[startAngle+1];
		iTabPremierPolyy = new int[startAngle+1];
		
		int it = 1;
		
		double rad = 0;
		
		double PI = 3.14159264;
		
		g2.setStroke(new BasicStroke(5f));

		for(double i = 1.0f; i <= startAngle; i++)
		{
			rad = ((((360.0f/startAngle)*i)+Surplus) * PI)/180.0f;
			x = (r.width/2) - (Math.sin(rad) * (r.width/2));
			y = (r.height/2) - (Math.cos(rad) * (r.height/2));
			
			iTabPremierPolyx[it] = (int)x;
			iTabPremierPolyy[it] = (int)y;
			it++;

		//	g2.drawLine((int)oldx, (int)oldy, (int)x, (int)y);		//Ligne cote du polygone
			
		//	g2.drawLine((r.width/2), (r.height/2), (int)x, (int)y); //du centre vers le pic du polygone
			oldx = x;
			oldy = y;
		}

		double oldrad = ((((360.0f/startAngle)/2.0f)+Surplus) * PI)/180.0f;
		oldx = (r.width/2) - (Math.sin(oldrad) * (r.width/6));
		oldy = (r.height/2) - (Math.cos(oldrad) * (r.height/6));
		
		int iTabDeuxPolyx[];
		int iTabDeuxPolyy[];
		iTabDeuxPolyx = new int[startAngle+1];
		iTabDeuxPolyy = new int[startAngle+1];

		for(double j = 1.0f; j <= startAngle+1; j++)
		{
			g2.drawLine((int)oldx, (int)oldy, iTabPremierPolyx[(int)j], iTabPremierPolyy[(int)j]);
			
			rad = oldrad + ((((360.0f/startAngle)*j)/*+Surplus*/) * PI)/180.0f;
			x = /*Math.round(*/(r.width/2) - (Math.sin(rad) * (r.width/6));
			y = /*Math.round(*/(r.height/2) - (Math.cos(rad) * (r.height/6));
			
			g2.setStroke(new BasicStroke(2f));
			g2.drawLine((int)oldx, (int)oldy, (int)x, (int)y);
			g2.setStroke(new BasicStroke(5f));
			
			g2.drawLine((int)x, (int)y, iTabPremierPolyx[(int)j], iTabPremierPolyy[(int)j]);

			//	g2.drawLine((r.width/2), (r.height/2), (int)x, (int)y);
			oldx = x;
			oldy = y;
		}
    }

    public void redraw(boolean clear, int start, int end) 
	{
		this.clear = clear;
		if(clear)
		{
			update(getGraphics());
			initDraw(getGraphics());
			return;
		}
		this.startAngle = start;
		this.onlyAngle = start;
		this.endAngle = end;
		
	//	tabFormes[iNbForme] =  startAngle;
	//	iNbForme++;

		//On peint!
		this.paint2(this.getGraphics(), frame);
    }
	
	public void initDraw(Graphics g)
	{
		Rectangle r = getBounds();
		
	///	int hlines = r.height / 10;
	//	int vlines = r.width / 10;
		
	//	g.setColor(Color.pink);
		/*for (int i = 1; i <= hlines; i++)
		{
			g.drawLine(0, i * 20, r.width, i * 20);
		}
		for (int i = 1; i <= vlines; i++) 
		{
			g.drawLine(i * 20, 0, i * 20, r.height);
		}*/
		
		g.setColor(Color.red);

		g.drawArc(0, 0, r.width - 1, r.height - 1, 0, 360);
		
		g.drawLine(r.width/2, r.height/2, r.width/2, r.height/2);
		
		
	//	g.setColor(Color.black);
	//	g.drawLine(0, r.height / 2, r.width, r.height / 2);
	//	g.drawLine(r.width / 2, 0, r.width / 2, r.height);
	}
}


class ArcControls extends Panel
                  implements ActionListener 
{
    TextField s;
    TextField e;
    ArcCanvas canvas;

    public ArcControls(ArcCanvas canvas) 
	{
		Button b = null;

		this.canvas = canvas;
		add(s = new TextField("0", 4));
		b = new Button("Clear Last");
		b.addActionListener(this);
		add(b);
		b = new Button("Draw");
		b.addActionListener(this);
		add(b);
    }

    public void actionPerformed(ActionEvent ev) 
	{
		String label = ev.getActionCommand();
			
		canvas.redraw(label.equals("Clear Last"), Integer.parseInt(s.getText().trim()), 360);

    }
}

