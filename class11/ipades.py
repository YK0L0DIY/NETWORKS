def how_many(ip1, ip2):
    n = ip1 - ip2
    return pow(2, n)


# print(how_many(int(input("ip/")), int(input("network/"))))
def contains(ip, mask, host):
    i = ''.join([bin(int(x) + 256)[3:] for x in ip.split('.')])
    j = ''.join([bin(int(x) + 256)[3:] for x in host.split('.')])
    for x in range(0, mask):
        if i[x] != j[x]:
            return False
    return True


# print(contains('192.168.0.128', 28, '192.168.0.129'))
# print(contains('192.168.0.128', 28, '192.168.0.145'))
# print(contains('192.168.0.128', 28, '192.168.1.129'))
# print(contains('192.168.0.128', 28, '192.168.0.1'))
# print(contains('192.168.0.128', 28, '192.168.0.140'))


def list_mask_ips(ip):
    input = ip.split('/')
    i = ''.join([bin(int(x) + 256)[3:] for x in input[0].split('.')])

    sum = bin(int(i, 2))
    new = sum[2:]
    new_ip = f'{int(new[0:8], 2)}.{int(new[8:16], 2)}.{int(new[16:24], 2)}.{int(new[24:32], 2)}'
    while contains(input[0], int(input[1]), new_ip):
        print(new_ip)
        sum = bin(int(sum, 2) + int('1', 2))
        new = sum[2:]
        new_ip = f'{int(new[0:8], 2)}.{int(new[8:16], 2)}.{int(new[16:24], 2)}.{int(new[24:32], 2)}'


list_mask_ips('192.168.0.128/28')
