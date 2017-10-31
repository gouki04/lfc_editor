local file = io.open('test.html', 'r')
local content = file:read('*a')
file:close()

local all_urls = {}
string.gsub(content, 'href="([^"]+)"', function(url)
    table.insert(all_urls, url)
end)

local all_sizes = {}
string.gsub(content, '</td><td align="right">([0-9%. KM]+)</td><td>&nbsp;</td></tr>', function(size_str)
    local num = 0
    string.gsub(size_str, '([0-9%.]+)', function(num_str)
        num = tonumber(num_str)
    end)

    if string.find(size_str, 'K') then
        num = num * 1024
    elseif string.find(size_str, 'M') then
        num = num * 1024 * 1024
    end
    
    num = math.floor(num + 0.5)
    table.insert(all_sizes, num)
end)

file = io.open('url.bytes', 'w')
file:write(#all_urls)
file:write('\n')

for i = 1, #all_urls do
    file:write(all_urls[i])
    file:write('\n')
    file:write(all_sizes[i])
    file:write('\n')
end
file:flush()
file:close()