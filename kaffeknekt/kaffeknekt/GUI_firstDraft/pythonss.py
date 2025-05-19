import argparse
from PIL import Image

def main():
    parser = argparse.ArgumentParser(description='Resize a .webp image.')
    parser.add_argument('input', help='Path to the input .webp file')
    parser.add_argument('output', help='Path for the output .webp file')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-s', '--scale', type=float, 
                       help='Scaling factor (e.g., 0.5 to reduce size by half)')
    group.add_argument('-w', '--width', type=int, 
                       help='Target width in pixels (must be used with --height)')
    parser.add_argument('-H', '--height', type=int, 
                        help='Target height in pixels (required if --width is used)')
    parser.add_argument('-q', '--quality', type=int, default=80, 
                        help='Output quality (0-100, defaults to 80)')
    args = parser.parse_args()

    # Open the image
    with Image.open(args.input) as img:
        orig_w, orig_h = img.size

        # Compute new dimensions
        if args.scale is not None:
            new_w = int(orig_w * args.scale)
            new_h = int(orig_h * args.scale)
        else:
            if args.width is None or args.height is None:
                parser.error('Both --width and --height must be specified together.')
            new_w, new_h = args.width, args.height

        # Resize with high-quality filter
        resized = img.resize((new_w, new_h), Image.LANCZOS)

        # Save the resized image
        resized.save(args.output, format='WEBP', quality=args.quality)
        print(f"Resized image saved to '{args.output}' ({new_w}×{new_h}, quality={args.quality})")

if __name__ == '__main__':
    main()
