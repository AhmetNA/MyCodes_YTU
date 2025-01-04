import java.io.Serializable;
import java.util.LinkedList;

public class Section implements Serializable {
    private int id;
    private String name;
    private LinkedList<Doctor> doctors;
    private LinkedList<Patient> patients;

    public Section(int id, String name){
        this.id = id;
        this.name = name;
        doctors = new LinkedList<>();
        patients = new LinkedList<>();
    }

    @Override
    public String toString() {
        return ("Section Name: " + name + " Section ID: " + id);
    }

    public int getId() {return id;}

    public String getName() {return name;}

    public void addDoctor(Doctor doctor) throws DuplicateInfoException{
        if(doctors.contains(doctor)){
            throw new DuplicateInfoException("Doctor already exists");
        }

        // Throw doctor exist regard to diplomaID
        for(Doctor d : doctors){
            if(d.getDiplomaID() == doctor.getDiplomaID()){
                throw new DuplicateInfoException("Doctor already exists");
            }
        }

        doctors.add(doctor);
    }

    public Doctor getDoctor(int diploma_id){
        for (Doctor doctor : doctors) {
            if (doctor.getDiplomaID() == diploma_id) {
                return doctor;
            }
        }
        System.out.println("Doctor not found");
        return null;
    }

    public void addPatient(Patient patient){
        patients.add(patient);
    }

    public LinkedList<Rendezvous> getRendezvousList() {
        LinkedList<Rendezvous> list = new LinkedList<>();
        for (Doctor doctor : doctors) {
            list.addAll(doctor.getSchedule().getRendezvousList());
        }
        return list;
    }

    public LinkedList<Doctor> getDoctors() {
        return doctors;
    }

    public Doctor getDoctorByName(String selectedDoctorName) {
        for (Doctor doctor : doctors) {
            if (doctor.getName().equals(selectedDoctorName)) {
                return doctor;
            }
        }
        return null;
    }


}
