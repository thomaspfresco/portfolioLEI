package com.example.demo;    

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import com.example.data.Match;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service    
public class MatchService   
{    
    @Autowired    
    private MatchRepository matchRepository;

    public List<Match> getAllMatches()  
    {    
        List<Match>userRecords = new ArrayList<>();    
        matchRepository.findAll().forEach(userRecords::add);    
        return userRecords;    
    }

    public void addMatch(Match match)  
    {    
        matchRepository.save(match);    
    }

    public Optional<Match> getMatch(int id) {
        return matchRepository.findById(id);
    }

    public List<Match> findByNameEndsWith(String chars) {
        return matchRepository.findByNameEndsWith(chars);
    }

}    