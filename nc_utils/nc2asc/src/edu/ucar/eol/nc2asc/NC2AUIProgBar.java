package nc2asc;


import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import nc2ascData.NCData;

import java.beans.*;
import java.util.Random;

public class NC2AUIProgBar extends Dialog implements ActionListener, PropertyChangeListener {

	private JProgressBar progressBar;
	private JButton doneButton, cancelButton;
	private Task task;

	private int maxLen;
	private NCData ncdata;
	private int exitVal=-1;

	class Task extends SwingWorker<Void, Void> {
		/*
		 * Main task. Executed in background thread.
		 */
		@Override
		public Void doInBackground() {
			int progress = ncdata.getProgIdx();
			//Initialize progress property.
			setProgress(0);
			while (!ncdata.getFinish()) {
				//Sleep for up to one second.
				setProgress(Math.min(100*progress/maxLen, 100));
				try {
					Thread.sleep(1000);
				} catch (InterruptedException ignore) {}
				//Make random progress.
				//progress += random.nextInt(10);
				//setProgress(Math.min(100*progress/maxLen, 100));
				progress = ncdata.getProgIdx();
			}
			progress = maxLen;
			setProgress(100);
			doneButton.setEnabled(true);
			cancelButton.setEnabled(false);
			return null;
		}

		/*
		 * Executed in event dispatching thread
		 */
		@Override
		public void done() {
			Toolkit.getDefaultToolkit().beep();
			doneButton.setEnabled(true);
			setCursor(null); //turn off the wait cursor
			// taskOutput.append("Done!\n");
		}
	}


	public NC2AUIProgBar(Frame owner, boolean modal, NCData ncd, int max) {
		super(owner, modal);
		super.setLayout(new GridLayout(4,1));
		JDialog.setDefaultLookAndFeelDecorated(true);
		this.setTitle("   Data Processing   ");
	
		maxLen =max;
		ncdata = ncd;
		
		//Create the demo's UI.
		doneButton = new JButton("Done");
		doneButton.setActionCommand("done");
		doneButton.addActionListener(this);

		cancelButton = new JButton("Cancel");
		cancelButton.setActionCommand("cancel");
		cancelButton.addActionListener(this);

		progressBar = new JProgressBar(0, 100);
		progressBar.setValue(0);
		progressBar.setBorderPainted(true);
		
		int idx =0;
		add(new JLabel(""),idx++);
		add(progressBar, idx++);
		add(new JLabel(""),idx++);

		JPanel jp = new JPanel();
		jp.setLayout(new GridLayout(0,3));
		jp.add(new JLabel(""));
		jp.add(cancelButton);
		jp.add(doneButton);
		add(jp);
		 
		setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
		task = new Task();
		task.addPropertyChangeListener(this);
		task.execute();
	}


	/**
	 * Invoked when the user presses the start button.
	 */
	public void actionPerformed(ActionEvent e) {
		if ("done".equals(e.getActionCommand())) {
			exitVal=0;
		} 
		if ("cancel".equals(e.getActionCommand())) {
			exitVal= -1;
			ncdata.setFinish(true);
		} 
	}

	/**
	 * Invoked when task's progress property changes.
	 */
	public void propertyChange(PropertyChangeEvent evt) {
		if ("progress" == evt.getPropertyName()) {
			int progress = (Integer) evt.getNewValue();
			progressBar.setValue(progress);
			//taskOutput.append(String.format(
			//		"Completed %d%% of task.\n", task.getProgress()));
		} 
	}

	public int getExitVal() {
		return exitVal;
	}

}
