time = 0
10_000.times do
  time += `./exec/rrb #{ARGV[0]}`.to_f
end
p time / 10_000
