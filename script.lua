function update()
    if right then
        player.x = player.x + 1;
    end
    if left then
        player.x = player.x - 1;
    end
    if up then
        player.y = player.y - 1;
    end
    if down then
        player.y = player.y + 1;
    end
end
