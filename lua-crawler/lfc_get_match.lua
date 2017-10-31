local http = require 'socket.http'
local content = http.request('http://app.gooooal.com/teamAllMatch.do?sid=2015&tid=53&t=0&lang=cn')

--[[
<td class="right"><a href="javascript:toTeam(28,11)">多特蒙德</a></td>
<td class="center"><a href="javascript:toEvents(1198066)">1-1</a></td>
<td><a href="javascript:toTeam(53,11)"><span class="green">利物浦</span></a></td>
]]

content = string.gsub(content, '<span[^>]+>', '')

local lfc_team_id = 53
local all_match = {}
string.gsub(content, '<td>(%d%d%d%d%-%d%d%-%d%d)</td>[^:]+:toTeam%((%d+),%d+%)"%s*>([^<]+)<[^:]+:toEvents%((%d+)%)"%s*>([^<]+)<[^:]+:toTeam%((%d+),%d+%)"%s*>([^<]+)<', 
  function(date, home_team_id, home_team, match_id, result, away_team_id, away_team)
    match_id = tonumber(match_id)
    home_team_id = tonumber(home_team_id)
    away_team_id = tonumber(away_team_id)
    
    if all_match[match_id] then
      return
    end
    
    local opponent_team, side
    if home_team_id == lfc_team_id then
      opponent_team = {
          id = away_team_id,
          name = away_team,
        }
      side = 'home'
    else
      opponent_team = {
          id = home_team_id,
          name = home_team,
        }
      side = 'away'
    end
    
    all_match[match_id] = {
        id = match_id,
        date = date,
        side = side,
        opponent_team = opponent_team,
        result = string.format('%s_%s_%s_%s', date, home_team, result, away_team),
      }
  end)

local dir = 'E:\\lfc\\2015\\'
for id, match in pairs(all_match) do
  local match_content = http.request(string.format('http://app.gooooal.com/sports/football/analysis/event.do?matchId=%d&lang=cn', id))
  local file_name = dir .. match.date .. '.html'
  local f, err = io.open(file_name, 'w')
  f:write(match_content)
  f:flush()
  f:close()
end