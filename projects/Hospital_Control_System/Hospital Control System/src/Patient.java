public class Patient extends Person{

    public Patient(String name, long Id){
        super(name, Id);
    }

    @Override
    public String toString() {
        return "Patient" + " " + super.getName() + " " + super.getNational_Id();
    }
    public String getName() {
        return super.getName();
    }
    public long getNational_Id() {
        return super.getNational_Id();
    }
}
