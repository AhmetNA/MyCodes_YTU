import java.io.Serializable;
import java.util.LinkedList;

public class Hospital implements Serializable {
    private final int id;
    private String name;
    private LinkedList<Section> sections;

    public Hospital(int id , String name){
        this.id = id;
        this.name = name;
        sections = new LinkedList<>();
    }

    public Section getSection(int id) {
        for (Section section : sections) {
            if (section.getId() == id) {
                return section;
            }
        }
        System.out.println("Section not found");
        return null;
    }

    private Section getSections(String name) {
        for (Section section : sections) {
            if (section.getName().equals(name)) {
                return section;
            }
        }
        return null;
    }

    public int getId() {
        return id;
    }

    public void addSection(String name) {
        // Get last section id
        int id = 1;
        if (!sections.isEmpty()) {
            id = sections.getLast().getId() + 1;
        }
        sections.add(new Section(id, name));
    }

    public LinkedList<Rendezvous> getAllRendezvous() {
        LinkedList<Rendezvous> rendezvous = new LinkedList<>();
        for (Section section : sections) {
            rendezvous.addAll(section.getRendezvousList());
        }
        return rendezvous;
    }

    public LinkedList<Doctor> getAllDoctors(){
        LinkedList<Doctor> doctors = new LinkedList<>();
        for (Section section : sections) {
            doctors.addAll(section.getDoctors());
        }
        return doctors;
    }

    public LinkedList<Section> getAllSections(){
        return sections;
    }

    public Section getSectionByName(String selectedSectionName) {
        for (Section section : sections) {
            if (section.getName().equals(selectedSectionName)) {
                return section;
            }
        }
        return null;
    }
    
}
