import java.io.Serializable;
import java.text.ParseException;
import java.util.Date;
import java.util.LinkedList;

public class Schedule implements Serializable {
    private final int maxPatientsPerDay = 12;
    private LinkedList<Rendezvous> rendezvousList;

    public Schedule(){
        rendezvousList = new LinkedList<>();
    }

    public void addRendezvous(Patient patient, Date date, Doctor doctor) throws ParseException {
        if(doctor.isAvailable(date)){
            rendezvousList.add(new Rendezvous(patient, date, doctor, doctor.getSection()));
            // Add patient to section
            doctor.getSection().addPatient(patient);
        }
    }

    public void listRendezvous() throws ParseException {
        for (Rendezvous r : rendezvousList) {
            System.out.println("Section: " + r.getSection().getName() + " Doctor: " + r.getDoctor().getName() + " Patient: " + r.getPatient().getName() + " Date: " + r.getDate());
        }
    }

    public LinkedList<Rendezvous> getRendezvousList() {
        return rendezvousList;
    }

    public int getMaxPatientsPerDay() {return maxPatientsPerDay;}

}
