-- parse match data

-- logId
-- 1 进球 assistId 助攻球员
-- 2 乌龙
-- 3 点球（罚进）
-- 104 换人（下场）
-- 105 换人（上场） id和104对应
-- 106 受伤
-- 202 黄牌
-- 203 红牌
-- 205 罚丢点球

-- periodId
-- 100 上半场
-- 102 下半场
-- 103 加时赛
-- 104 点球大战

local file = io.open('data.txt')

local lfc_id = 53
local side = 'home'
local lfc_player = {}

local opponent_team = nil
local opponent_player = {}

local date = nil

local content = file:read('*a')

--[[
开赛时间 2017年09月09日
]]
string.gsub(content, '开赛时间 (%d+)年(%d+)月(%d+)日', function(y,m,d)
      date = {
          year = tonumber(y),
          month = tonumber(m),
          day = tonumber(d),
        }
    end)
  
--[[
var _hostTeam = ["216","曼城","http://pic.gooooal.com/images/100838/100838782.png","123339","Manchester City FC","1"];
]]
string.gsub(content, 'var%s+_hostTeam%s+=%s%[([^;]+)%]', function(str)
      local t = loadstring('return {' .. str .. '}')()
      if t[1] == tostring(lfc_id) then
        side = 'home'
      else
        opponent_team = {
            id = tonumber(t[1]),
            name = t[2],
            en_name = t[5],
          }
      end
    end)

--[[
var _visitTeam = ["53","利物浦","http://pic.gooooal.com/images/100011/100011218.jpg","100009327","Liverpool FC","1"];
]]
string.gsub(content, 'var%s+_visitTeam%s+=%s%[([^;]+)%]', function(str)
      local t = loadstring('return {' .. str .. '}')()
      if t[1] == tostring(lfc_id) then
        side = 'away'
      else
        opponent_team = {
            id = tonumber(t[1]),
            name = t[2],
            en_name = t[5],
          }
      end
    end)

--[[
_personListHost["107179"] = {pid:107179,teamId:216,name:"S.阿奎罗",teamNum:"10",posId:"17",posCode:"F",posName:"前锋",clubId:"216",clubName:"曼城",birthday:"1988-06-02",height:"173",marketValue:"6000万欧元",imgUrl:"http://pic.gooooal.com/images/101115/101115454.jpg",countryImg:"http://pic.gooooal.com/images/100490/100490028.png",countryName:"阿根廷"};
]]
string.gsub(content, '_personList([%w]+)%["(%d+)"%]%s+=%s+({[^}]+})', function(_side, id, str)
      str = string.gsub(str, ':', '=')
      local t = loadstring('return '..str)()
      if _side == 'Host' then
        if side == 'home' then
          lfc_player[id] = t
        else
          opponent_player[id] = t
        end
      else
        if side == 'away' then
          lfc_player[id] = t
        else
          opponent_player[id] = t
        end
      end
      --print (_side, id, t.name)
    end)
  
--[[
_logList.push({id:108106577,teamId:53,periodId:100,logId:202,logTime:1140,personId:100362176,assistId:"",remark:" Alexander-Arnold T.",reason:""});
]]
string.gsub(content, '_logList.push%(({[^}]+})%)', function(str)
      str = string.gsub(str, ':', '=')
      local t = loadstring('return '..str)()
      --print (t.personId)
    end)
  
print (date.day)
print (side)
print (opponent_team.en_name)