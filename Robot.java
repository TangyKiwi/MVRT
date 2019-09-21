package frc.robot;
import edu.wpi.first.wpilibj.IterativeRobot;
import edu.wpi.first.wpilibj.command.Command;
import edu.wpi.first.wpilibj.command.Scheduler;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.DigitalOutput;
import edu.wpi.first.wpilibj.I2C;

public class Robot extends IterativeRobot 
{
	public static String lastSend;
	public static I2C arduino;

	// updates Kevin's Multiple Led stip, led strip that can be individually
	// programmed to turn a specific color, can have multiple colors at once
	public static void updateLEDs(String WriteString)
	{
		if(WriteString.equals(lastSend)) return;
	    lastSend = WriteString;
		char[] CharArray = WriteString.toCharArray();
		byte[] WriteData = new byte[CharArray.length];
		for (int i = 0; i < CharArray.length; i++) 
		{
			WriteData[i] = (byte) CharArray[i];
		}
		arduino.writeBulk(WriteData, WriteData.length);
	}
	
  	public static void updateLEDsEnabled()
	{
		Robot.updateLEDs("DRIVING");
	}

	public static void updateLEDsDisabled()
	{
		Robot.updateLEDs("DISABLED");
	}

	public void disabledInit()
	{
		// when robot is disabled, they two strips will change color
		updateLEDsDisabled();
	}

	@Override
	public void teleopInit()
	{
		// when robot is enabled, they two strips will change color
		updateLEDsEnabled();
	}
	public void robotInit() {
		lastSend = "nothing";
		arduino = new I2C(I2C.Port.kOnboard, 128);
		oi = new OI();
		SmartDashboard.putData("Auto mode", chooser);
	}
	
	// end the copying here
	// ingore the rest, it is not needed for leds
	// if you have other code in methods such as robotInit() do not delete and replece the method
	// just add the code we added into robotInit() to your existing code
	public static OI oi;
	public int width = 10;
	Command autonomousCommand;
	SendableChooser<Command> chooser = new SendableChooser<>();

	@Override
	public void disabledPeriodic() { Scheduler.getInstance().run(); }
	@Override
	public void autonomousInit() {
		autonomousCommand = chooser.getSelected();
		if (autonomousCommand != null) autonomousCommand.start();
	}
	@Override
	public void autonomousPeriodic() { Scheduler.getInstance().run(); }
	@Override
	public void teleopPeriodic() { Scheduler.getInstance().run(); }
	@Override
	public void testPeriodic() {}
}