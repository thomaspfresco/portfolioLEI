package com.example.demo;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import com.example.data.User;
import com.example.data.Team;
import com.example.data.Player;
import com.example.data.Event;
import com.example.data.Match;
//import com.example.formdata.FormData;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;

import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.JsonNode;
import com.mashape.unirest.http.Unirest;
import com.mashape.unirest.http.exceptions.UnirestException;

import org.json.JSONArray;
import org.json.JSONObject;

@Controller
public class DataController {

    User actual = null;

    @Autowired
    TeamService teamService;

    @Autowired
    PlayerService playerService;

    @Autowired
    MatchService matchService;

    @Autowired
    EventService eventService;

    @Autowired
    UserService userService;

    @GetMapping("/")
    public String landingPage() {

        // if (this.actual == null) return "redirect:/login";
        return "redirect:/home";
    }

    @GetMapping("/home")
    public String home(@ModelAttribute User user, Model model) {
        List<User> u = this.userService.findByLogged(true);
        if (u.size() == 1) {
            model.addAttribute("user", userService.findByLogged(true));
            if (u.get(0).getAdmin() == true) {
                return "homeAdmin";
            }
            return "homeRegistered";
        }
        return "home";
    }

    @GetMapping("/createData")
    public String createData() {
        return "createData";
    }

    @PostMapping("/saveData")
    public String saveData(Model model) throws UnirestException {
        
        /*Team[] myteams = {
         new Team("Benfica"),
         new Team("Porto"),
         new Team("Sporting")
        };
        Player[] myplayers = {
         new Player("Miguel", "Defesa", "06/06/2001"),
         new Player("Thomas", "Atacante", "08/02/2001"),
         new Player("Diogo", "Medio", "25/12/1999"),
         new Player("Tiago", "Guarda-redes", "20/10/2000")
        };
         
        Match[] mymatches = {
         new Match(myteams[0], myteams[1], "Febres", "10/05/2022", false),
         new Match(myteams[1], myteams[0], "Manteigas", "20/02/2022", false),
         new Match(myteams[0], myteams[2], "Lisboa", "01/02/2022", true),
         new Match(myteams[1], myteams[2], "Porto", "05/02/2022", true)
         
        };
         
        Event[] myevents = {
         new Event(mymatches[0], myplayers[1], "Golo", 17),
         new Event(mymatches[1], myplayers[2], "Jogo Interrompido", 17),
         new Event(mymatches[2], myplayers[3], "Golo", 88),
         new Event(mymatches[2], myplayers[3], "Golo", 89),
         new Event(mymatches[3], myplayers[2], "Golo", 51),
        };
         
        User[] myusers = {
         new User("thomaspfresco", "bananas", "thomasfresco@gmail.com", 939929919,
         false),
         new User("miguelferreira", "tiktok", "mafarrico@gmail.com", 934567123, true)
        };
        
        myplayers[0].setTeam(myteams[0]);
        myplayers[1].setTeam(myteams[0]);
        myplayers[2].setTeam(myteams[1]);
        myplayers[3].setTeam(myteams[2]);
        
        myteams[0].addPlayer(myplayers[0]);
        myteams[0].addPlayer(myplayers[1]);
        myteams[1].addPlayer(myplayers[2]);
        myteams[2].addPlayer(myplayers[3]);
        
        myteams[2].addWins();
        myteams[0].addLosses(); //sporting ganhou ao benfica mymatches[2]
        myteams[1].addWins();
        myteams[2].addLosses(); //porto ganhou ao sporting mymatches[3]
        
        for (Player s : myplayers)
         this.playerService.addPlayer(s);
        
        for (Team t : myteams)
         this.teamService.addTeam(t);
        
        for (Match m : mymatches)
         this.matchService.addMatch(m);
         
        for (Event e : myevents)
         this.eventService.addEvent(e);
          
        for (User u : myusers)
         this.userService.addUser(u);
        */


        User[] myusers = {
                new User("thomaspfresco", "bananas", "thomasfresco@gmail.com", 939929919, false),
                new User("miguelferreira", "tiktok", "mafarrico@gmail.com", 934567123, true)
        };

        for (User u : myusers)
            this.userService.addUser(u);

        String key = "059404dd8941d2fac8a1abbe12633fb1";
        String link = "https://v3.football.api-sports.io/";
        String endpoint = "players?league=39&season=2021";

        HttpResponse<JsonNode> response = Unirest.get(link + endpoint)
                .header("x-rapidapi-key", key)
                .header("x-rapidapi-host", "v3.football.api-sports.io")
                .asJson();

        JSONArray playersAux = response.getBody().getObject().getJSONArray("response");

        ArrayList<JSONObject> players = new ArrayList<>();
        for (int i = 0; i < playersAux.length(); i++) {
            players.add(playersAux.getJSONObject(i));
        }

        List<Team> myteams = new ArrayList<>();
        List<Player> myplayers = new ArrayList<>();
        //int jogador = 0;
        //int equipa = 0;
        for (int i = 0; i < players.size(); i++) {
            JSONObject p = players.get(i);

            String teamName = p.optJSONArray("statistics").getJSONObject(0).getJSONObject("team").getString("name");

            List<Team> teamList = this.teamService.getTeamByName(teamName);
            Team team = null;
            if (teamList.size() == 0)
                team = new Team(teamName);
            else
                team = teamList.get(0);

            Player player = new Player(p.getJSONObject("player").getString("name"),
                    p.optJSONArray("statistics").getJSONObject(0).getJSONObject("games").getString("position"),
                    p.getJSONObject("player").getJSONObject("birth").optString("date", "Unknown"));
            player.setTeam(team);
            this.playerService.addPlayer(player);
            myplayers.add(player);
            //System.out.println(player.getName() + " -" + jogador + "- " + team.getName());
            //jogador++;

            team.addPlayer(player);
            this.teamService.addTeam(team);
            if(!myteams.contains(team)) {
                myteams.add(team);
                //System.out.println("                " + team.getName() + " --" + equipa);
                //equipa++;
            }

            
        }

        Match[] mymatches = {
            new Match(myteams.get(0), myteams.get(1), "Anfield", "2022-05-22", true), //liverpool v arsenal
            new Match(myteams.get(0), myteams.get(2), "Liverpool", "2022-05-10", true), //liverpool v everton
            new Match(myteams.get(5), myteams.get(6), "Porto", "2022-06-01", false), //newcastle v norwich
            new Match(myteams.get(8), myteams.get(7), "Lisboa", "2022-06-01", false)  //leicester v wolves    
        };

        for (Match m : mymatches)
         this.matchService.addMatch(m);
         
        Event[] myevents = {
            new Event(mymatches[0], myplayers.get(18), "Golo", 20), //J. Danns
            new Event(mymatches[0], myplayers.get(0), "Fim de Jogo", 90),
            new Event(mymatches[1], myplayers.get(18), "Golo", 51), //J. Danns
            new Event(mymatches[1], myplayers.get(18), "Golo", 88), //J. Danns
            new Event(mymatches[1], myplayers.get(0), "Fim de Jogo", 90),
            new Event(mymatches[2], myplayers.get(11), "Cartao Amarelo", 70), //Thomas Dickson-Peters
            new Event(mymatches[3], myplayers.get(12), "Golo", 22) //Christian Fernandes Marques

            //new Event(mymatches[2], myplayers[3], "Golo", 88),
        };

        for (Event e : myevents)
         this.eventService.addEvent(e);


        //atualizar wins e losses das equipas de acordo com os jogos
        myteams.get(0).addWins();
        myteams.get(0).addWins();
        myteams.get(1).addLosses();
        myteams.get(2).addLosses();

        for (Team t : myteams)
         this.teamService.addTeam(t);

        return "redirect:/home";
    }

    @PostMapping("/logout")
    public String logout(@ModelAttribute User u) {
        List<User> utilizador = this.userService.findByLogged(true);
        this.userService.updateLogin(utilizador.get(0).getId(), false);
        return "redirect:/home";
    }

    @GetMapping("/loginUser")
    public String loginUser(Model model) {
        model.addAttribute("user", new User());
        List<User> u = this.userService.findByLogged(true);
        if (u.size() == 1)
            return "redirect:/home";
        return "login";
    }

    @GetMapping("/login")
    public String login(@ModelAttribute User user, Model model) {
        model.addAttribute("user", new User());

        List<User> u2 = this.userService.findByLogged(true);
        if (u2.size() == 1)
            return "redirect:/home";

        List<User> u = this.userService.findByUsername(user.getUsername());

        if (u.size() == 1) {
            if (u.get(0).getPassword().equals(user.getPassword())) {
                // this.userService.addUser(user);
                this.userService.updateLogin(u.get(0).getId(), true);
                return "redirect:/home";
            }
            return "redirect:/loginUser";
        }

        return "redirect:/loginUser";
    }

    @GetMapping("/registUser")
    public String registUser(Model model) {
        model.addAttribute("user", new User());

        List<Boolean> adminOption = new ArrayList<>();
        adminOption.add(false);
        adminOption.add(true);
        model.addAttribute("adminOption", adminOption);

        return "regist";
    }

    @GetMapping("/regist")
    public String regist(@ModelAttribute User user, Model model) {
        model.addAttribute("user", new User());

        return "redirect:/home";
    }

    @GetMapping("/editUser")
    public String editUser(@RequestParam(name = "id", required = true) int id, Model m) {
        Optional<User> op = this.userService.getUser(id);
        if (op.isPresent()) {
            m.addAttribute("user", op.get());
            List<Boolean> adminStatusList = new ArrayList<Boolean>();
            adminStatusList.add(true);
            adminStatusList.add(false);
            m.addAttribute("adminStatus", adminStatusList);
            return "editUser";
        } else {
            return "redirect:/Home";
        }
    }

    @PostMapping("/saveUser")
    public String saveUser(@ModelAttribute User u) {
        List<User> utilizador = this.userService.findByUsername(u.getUsername());
        if (utilizador.size() == 0) {
            // System.out.println("entrei");
            this.userService.addUser(u);
            return "redirect:/home";
        }
        return "redirect:/registUser";
    }

    @GetMapping("/listPlayers")
    public String listStudents(Model model) {
        model.addAttribute("players", this.playerService.getAllPlayers());
        List<User> u = this.userService.findByLogged(true);
        if (u.size() == 1) {
            if (u.get(0).getAdmin() == true) {
                return "listPlayersAdmin";
            }
            return "listPlayersRegist";
        }
        return "listPlayers";
    }

    @GetMapping("/createPlayer")
    public String createPlayer(Model m) {
        m.addAttribute("player", new Player());
        m.addAttribute("allTeams", this.teamService.getAllTeams());
        return "editPlayer";
    }

    @GetMapping("/editPlayer")
    public String editPlayer(@RequestParam(name = "id", required = true) int id, Model m) {
        Optional<Player> op = this.playerService.getPlayer(id);
        if (op.isPresent()) {
            m.addAttribute("player", op.get());
            m.addAttribute("allTeams", this.teamService.getAllTeams());
            return "editPlayer";
        } else {
            return "redirect:/listPlayers";
        }
    }

    @PostMapping("/savePlayer")
    public String savePlayer(@ModelAttribute Player p) {
        this.playerService.addPlayer(p);
        return "redirect:/listPlayers";
    }

    @GetMapping("/createMatch")
    public String createMatch(Model m) {
        m.addAttribute("match", new Match());
        m.addAttribute("allTeams", this.teamService.getAllTeams());
        return "editMatch";
    }

    @GetMapping("/editMatch")
    public String editmatch(@RequestParam(name = "id", required = true) int id, Model m) {
        Optional<Match> op = this.matchService.getMatch(id);
        if (op.isPresent()) {
            m.addAttribute("match", op.get());
            m.addAttribute("allTeams", this.teamService.getAllTeams());
            return "editMatch";
        } else {
            return "redirect:/listMatches";
        }
    }

    @PostMapping("/saveMatch")
    public String saveMatch(@ModelAttribute Match m) {
        this.matchService.addMatch(m);
        return "redirect:/listMatches";
    }

    @GetMapping("/listMatchEvents")
    public String listMatchEvents(@RequestParam(name = "id", required = true) int id, Model m) {
        Optional<Match> op = this.matchService.getMatch(id);
        if (op.isPresent()) {
            m.addAttribute("match", op.get());
            m.addAttribute("thisEvents", this.eventService.findEventsFromMatch(id));

            Team teamA = op.get().getTeamA();
            List<Event> goals = this.eventService.goalsInMatch(id);
            int goalsA = 0;
            int goalsB = 0;
            for (int i = 0; i < goals.size(); i++) {
                if (goals.get(i).getType().equals("Golo")) {
                    if (teamA.getName().equals(goals.get(i).getPlayer().getTeam().getName()))
                        goalsA++;
                    else
                        goalsB++;
                }
            }
        
            String resultado = teamA.getName() + " " + goalsA + " - " + goalsB + " " + op.get().getTeamB().getName();
            m.addAttribute("resultado", resultado);

            List<User> u = this.userService.findByLogged(true);
            if (u.size() == 1) {
                if (u.get(0).getAdmin() == true)
                    return "listMatchEventsAdmin";
                else
                    return "listMatchEventsRegist";
            }

            return "listMatchEvents";
        } else {
            return "redirect:/listMatches";
        }
    }

    @GetMapping("/listTeams")
    public String listTeams(Model model) {
        model.addAttribute("teams", this.teamService.getAllTeams());
        List<User> u = this.userService.findByLogged(true);
        if (u.size() == 1) {
            if (u.get(0).getAdmin() == true) {
                return "listTeamsAdmin";
            }
            return "listTeamsRegist";
        }
        return "listTeams";
    }

    @GetMapping("/createTeam")
    public String createTeam(Model m) {
        m.addAttribute("team", new Team());
        return "editTeam";
    }

    private String getEditTeamForm(int id, String formName, Model m) {
        Optional<Team> op = this.teamService.getTeam(id);
        if (op.isPresent()) {
            m.addAttribute("team", op.get());
            return formName;
        }
        return "redirect:/listTeams";
    }

    @GetMapping("/editTeam")
    public String editTeam(@RequestParam(name = "id", required = true) int id, Model m) {
        return getEditTeamForm(id, "editTeam", m);
    }

    @PostMapping("/saveTeam")
    public String saveTeam(@ModelAttribute Team t) {
        this.teamService.addTeam(t);
        return "redirect:/listTeams";
    }

    @GetMapping("/listEvents")
    public String listEvents(Model model) {
        model.addAttribute("events", this.eventService.getAllEvents());
        return "listEvents";
    }

    @GetMapping("/createEvent")
    public String createEvent(Model m) {
        m.addAttribute("event", new Event());
        m.addAttribute("allPlayers", this.playerService.getAllPlayers());
        m.addAttribute("allMatches", this.matchService.getAllMatches());

        List<String> types = new ArrayList<String>();
        types.add("Inicio de Jogo");
        types.add("Fim de Jogo");
        types.add("Golo");
        types.add("Cartao Amarelo");
        types.add("Cartao Vermelho");
        types.add("Jogo Interrompido");
        types.add("Jogo resumido");
        m.addAttribute("eventTypes", types);

        List<User> u = this.userService.findByLogged(true);
        if (u.size() == 1) {
            if (u.get(0).getAdmin() == true) {
                return "editEventAdmin";
            }
            return "editEvent";
        }

        return "redirect:/home";
    }

    /*
     * @GetMapping("/editEvent")
     * public String editEvent(@RequestParam(name = "id", required = true) int id,
     * Model m) {
     * Optional<Event> op = this.eventService.getEvent(id);
     * if (op.isPresent()) {
     * m.addAttribute("event", op.get());
     * m.addAttribute("allMatches", this.matchService.getAllMatches());
     * return "editEvent";
     * } else {
     * return "redirect:/listEvents";
     * }
     * }
     */

    @PostMapping("/saveEvent")
    public String saveEvent(@ModelAttribute Event e) {
        this.eventService.addEvent(e);
        if (e.getType().equals("Fim de Jogo")) {
            e.getMatch().setFinished(true);

            Team teamA = e.getMatch().getTeamA();
            List<Event> goals = this.eventService.goalsInMatch(e.getMatch().getId());
            int goalsA = 0;
            int goalsB = 0;
            for (int i = 0; i < goals.size(); i++) {
                if (goals.get(i).getType().equals("Golo")) {
                    if (teamA.getName().equals(goals.get(i).getPlayer().getTeam().getName()))
                        goalsA++;
                    else
                        goalsB++;
                }
            }

            if (goalsA > goalsB) {
                e.getMatch().getTeamA().addWins();
                e.getMatch().getTeamB().addLosses();
            } else if (goalsA < goalsB) {
                e.getMatch().getTeamB().addWins();
                e.getMatch().getTeamA().addLosses();
            } else {
                e.getMatch().getTeamB().addDraws();
                e.getMatch().getTeamA().addDraws();
            }

            this.matchService.addMatch(e.getMatch());
            this.teamService.addTeam(e.getMatch().getTeamA());
            this.teamService.addTeam(e.getMatch().getTeamB());
        }
        return "redirect:/home";
    }

    @GetMapping("/listMatches")
    public String listMatches(Model model) {
        model.addAttribute("matches", this.matchService.getAllMatches());
        List<User> u = this.userService.findByLogged(true);
        if (u.size() == 1) {
            if (u.get(0).getAdmin() == true) {
                return "listMatchesAdmin";
            }
            return "listMatchesRegist";
        }
        return "listMatches";
    }

    @GetMapping("/listUsers")
    public String listUsers(Model model) {
        model.addAttribute("users", this.userService.getAllUsers());
        return "listUsers";
    }

    @GetMapping("/statistics")
    public String statistics(Model m) {
        List<Player> players = this.playerService.getAllPlayers();

        m.addAttribute("allTeams", this.teamService.getAllTeams());

        Player topScorer = null;
        int topGoals = 0;
        int goals = 0;
        for (int i = 0; i < players.size(); i++) {
            goals = this.eventService.findGoalsByPlayer(players.get(i).getName()).size();
            if (goals > topGoals) {
                topGoals = goals;
                topScorer = players.get(i);
            }
        }

        m.addAttribute("topScorer", topScorer);
        List<User> u = this.userService.findByLogged(true);
        if (u.size() == 1) {
            if (u.get(0).getAdmin() == true) {
                return "statisticsAdmin";
            }
            return "statisticsRegist";
        }

        return "statistics";
    }

}