puts "1..13"
a=[10,18+2 , 30]
a[3] = 40
puts "#{a[0] == 10? 'ok' : 'nok'} 1 - a[0]"
puts "#{a[1] == 20? 'ok' : 'nok'} 2 - a[1]"
puts "#{a['2'] == 30? 'ok' : 'nok'} 3 - a['2']"
puts "#{a[1+2] == 40? 'ok' : 'nok'} 4 - a[1+2]"

n=0
for val in a do
    idx = val / 10  +  4
    n += 1
    puts "ok #{idx} - array iteration #{n}"
end

puts "#{%w{nok ok nok}[1]} 9 - quote words"
puts "#{['nok',['nok','nok','ok']][1][2]} 10 - 2d array"

def test_array_args(x, *arg_arr)
    puts "ok #{x} - fixed arg"
    for t in arg_arr do
       puts "ok #{t} - array slurpy arg"
    end
end

test_array_args(11, 12, 13)

