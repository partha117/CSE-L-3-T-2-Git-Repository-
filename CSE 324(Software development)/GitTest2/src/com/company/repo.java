package com.company;

/**
 * Created by ASUS on 26-Mar-17.
 */

import org.kohsuke.github.GHPersonSet;
import org.kohsuke.github.GHRepository;
import org.kohsuke.github.GHUser;
import org.kohsuke.github.GitHub;

import java.io.IOException;
import java.util.*;

import org.kohsuke.github.*;


import java.io.IOException;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class repo {

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
            //Map<String,GHRepository> repositoryMap=gitHub.getMyself().getAllRepositories();/*gitHub.getUser("partha117").getRepositories();*/
           GHRepository ghRepository=gitHub.getMyself().getRepository("Software_Development_CSE324");
            /*Set<String > key=repositoryMap.keySet();
            Iterator<String > stringIterator=key.iterator();
            for(;stringIterator.hasNext();)
            {
                String  temp=stringIterator.next();
                System.out.println(temp+" -> ");
                GHCommitQueryBuilder qb=repositoryMap.get(temp).queryCommits();*/
                List<GHCommit> l=ghRepository.queryCommits().list().asList();
                for(int i=0;i<l.size();i++)
                {
                    //System.out.println(l.get(i).getAuthor());
                    int line=l.get(i).getLinesAdded();
                    try {
                        String name = l.get(i).getCommitter().getLogin();
                        Date date = l.get(i).getCommitDate();
                        System.out.println(line + "   " + name + "   " + date.toString());
                    }catch (Exception e)
                    {

                    }
                }



            } catch (IOException e1) {
            e1.printStackTrace();
        }
    } /*catch (IOException e) {
            e.printStackTrace();
        }*/
}

