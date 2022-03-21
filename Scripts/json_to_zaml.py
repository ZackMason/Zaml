import json, yaml, argparse

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--file')
    parser.add_argument('-o', '--output')

    args = parser.parse_args()

    if not args.output:
        args.output.replace('.json', '.yaml')

    json_data = None
    with open(args.file) as f:
        json_data = json.load(f)
    
    zaml = to_zaml(json_data)

    with open(args.output, 'w+') as f:
        f.write(zaml)


def to_zaml(data, indent = 0, indent_scale = 2):
    yaml_data = ''

    if not indent:
        yaml_data += '---\n'

    spaces = ' ' * indent

    if type(data) == dict:
        for key in data.keys():
            value = data[key]
            value_type = type(value)
            if value_type in [int, float, bool, str]:
                yaml_data += f'{spaces}{key}: {value}\n'
            elif value_type in [list, dict]:
                yaml_data += f'{spaces}{key}: ~\n'
                yaml_data += to_zaml(data[key], indent + indent_scale)
            else:
                print(f'Unknown type {value_type} {value = }')
    elif type(data) == list:
        for item in data:
            if type(item) in [list, dict]:
                yaml_data += f'{spaces}-: ~\n'
                yaml_data += to_zaml(item, indent + indent_scale)
            else:
                yaml_data += f'{spaces}-: {item}\n'

    return yaml_data

if __name__ == '__main__':
    main()

