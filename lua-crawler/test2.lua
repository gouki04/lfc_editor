local file = io.open('all-folders-2012-03-24.opensudoku', 'r')
local content = file:read('*a')
file:close()

file = io.open('out.sudoku', 'w')
local sum = 0
for game in string.gmatch(content, '<game([^>]+)>') do
    for k,v in string.gmatch(game, '(%w+)="([^"]+)"') do
        if k == 'data' then
            if string.len(v) == 81 then
                file:write(v)
                file:write('\n')
            end
        end
    end
end

file:flush()
file:close()

--print (sum)