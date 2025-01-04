import java.io.Serializable;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Rendezvous implements Serializable {
    private Patient patient;
    private Date date;
    private Doctor doctor;
    private Section section;

    public Rendezvous(Patient patient, Date date, Doctor doctor, Section section) {
        this.patient = patient;
        this.date = date;
        this.doctor = doctor;
        this.section = section;
    }

    @Override
    public String toString() {
        return ("Date: " + getDateString()+ " Patient: " + patient.getName() + " Doctor: " + doctor.getName() + "Section" + getSection().getName());
    }

    public Section getSection() {
        return section;
    }

    public Doctor getDoctor() {
        return doctor;
    }

    public Person getPatient() {
        return patient;
    }

    public Date getDate() {
        return date;
    }

    public String getDateString() {
        SimpleDateFormat formatter = new SimpleDateFormat("dd/MM HH");
        return formatter.format(date);
    }
}