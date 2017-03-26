package com.company;

import org.kohsuke.github.*;


import java.io.IOException;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class Main {

    public static void main(String[] args) {
	// write your code here
        try {
            GitHub gitHub=GitHub.connectUsingPassword("partha117","chakraborty159");
            GHRepository repository=gitHub.getRepository("partha117/CSE-L-3-T-2-Git-Repository-");
           // System.out.println(repository.getDescription());
            /*GitHub gitHub1=GitHub.connectUsingPassword("Tester117","chakraborty159");
            repository.addCollaborators(gitHub1.getUser("Tester117"));*/
            /*GHPersonSet<GHUser> users= repository.getCollaborators();
            Iterator<GHUser> userIterator=users.iterator();
            for(;userIterator.hasNext();)
            {
                System.out.println(userIterator.next().getName());
            }*/
            Map<String,GHRepository> repositoryMap=gitHub.getMyself().getAllRepositories();/*gitHub.getUser("partha117").getRepositories();*/
            Set<String > key=repositoryMap.keySet();
            Iterator<String > stringIterator=key.iterator();
            for(;stringIterator.hasNext();)
            {
                String  temp=stringIterator.next();
                System.out.println(temp+" -> ");
                GHPersonSet<GHUser> users=repositoryMap.get(temp).getCollaborators();
                Iterator<GHUser> userIterator=users.iterator();
                for(;userIterator.hasNext();)
                {
                    GHUser tempuser=userIterator.next();
                    System.out.println("           "+tempuser.getName()+"( "+tempuser.getLogin()+" )");
                }

            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
