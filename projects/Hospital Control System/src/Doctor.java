import java.text.ParseException;
import java.util.Date;

public class Doctor extends Person{
    private int diploma_ID;
    private Schedule schedule;
    private Section section;

    public Doctor(String name, long NID, int diploma_Id, Section section) {
        super(name, NID);
        this.diploma_ID = diploma_Id;
        this.section = section;
        schedule = new Schedule();
    }
    @Override
    public String toString() {
         return ("Doctor: "+ getName()  + " Diploma ID = " + diploma_ID + " Section = " + section.getName());
    }

    public int getDiplomaID() {return diploma_ID;}

    public long getNational_Id() {
        return super.getNational_Id();
    }

    public Schedule getSchedule() {
        return schedule;
    }

    public boolean isAvailable(Date date) throws FullScheduleException {
        // Check if doctor has no patients
        if (schedule.getRendezvousList().isEmpty()) {
            return true;
        }

        // Check if doctor has reached the limit of patients per day
        int day_patients = 0;
        for(Rendezvous r : schedule.getRendezvousList()){
            if(r.getDate().getDay() == date.getDay()){
                day_patients++;
            }
        }
        if(day_patients >= schedule.getMaxPatientsPerDay()){
            throw new FullScheduleException("Doctor has reached the limit of patients per day");
        }

        // Check if doctor is available at this date and time
        for (Rendezvous r : schedule.getRendezvousList()) {
            if (r.getDate().getMonth() == date.getMonth() && r.getDate().getDay() == date.getDay() && r.getDate().getHours() == date.getHours()) {
                throw new FullScheduleException("Doctor is not available at this date and time");
            }
        }
        return true;
}

    public String getName() {
        return super.getName();
    }

    public Section getSection() {
        return section;
    }
}
