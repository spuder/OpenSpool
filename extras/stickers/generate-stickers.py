from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas
from reportlab.lib.units import inch
from PIL import Image
import io
import os
import tempfile
import shutil

# User modifiable parameters
LEFT_MARGIN = 3/8  # in inches
RIGHT_MARGIN = 3/8
TOP_MARGIN = 0.5
BOTTOM_MARGIN = 0.5
CIRCLE_DIAMETER = 1.0  # diameter of each circle in inches
NUM_COLUMNS = 7
NUM_ROWS = 9
LOGO_PATHS = ["OpenSpoolLogo1Inch.png", "BambuLogo1Inch.png"]
DPI = 300  # Increased DPI for better print quality

def get_output_filename(logo_path):
    """
    Generate output filename based on the input logo filename.
    """
    base_name = os.path.splitext(os.path.basename(logo_path))[0]
    return f"{base_name}.pdf"

def prepare_logo(logo_path, desired_size_inches):
    """
    Prepare the logo image for insertion into PDF with high resolution.
    
    Args:
        logo_path (str): Path to the logo PNG file
        desired_size_inches (float): Desired size in inches
        
    Returns:
        str: Path to the temporary logo file
    """
    # Create a temporary file with the same extension as the original
    temp_dir = tempfile.gettempdir()
    temp_logo_path = os.path.join(temp_dir, 'temp_logo.png')
    
    # Make a copy of the original logo
    shutil.copy2(logo_path, temp_logo_path)
    
    # Open the image
    img = Image.open(temp_logo_path)
    
    # Convert desired inches to pixels using higher DPI
    desired_size_pixels = int(desired_size_inches * DPI)
    
    needs_modification = False
    
    # Get original image dimensions
    original_width, original_height = img.size
    print(f"Original image size: {original_width}x{original_height} pixels")
    
    # Resize image if necessary, maintaining aspect ratio
    if img.size != (desired_size_pixels, desired_size_pixels):
        print(f"Resizing to {desired_size_pixels}x{desired_size_pixels} pixels")
        # Use high-quality resampling filter
        img = img.resize((desired_size_pixels, desired_size_pixels), 
                        Image.Resampling.LANCZOS)
        needs_modification = True
    
    # Convert to RGB if necessary (PDF doesn't support RGBA)
    if img.mode == 'RGBA':
        # Create a white background
        background = Image.new('RGB', img.size, (255, 255, 255))
        # Paste the image using alpha channel as mask
        background.paste(img, mask=img.split()[3])
        img = background
        needs_modification = True
    elif img.mode != 'RGB':
        img = img.convert('RGB')
        needs_modification = True
    
    # Save modifications to the temporary file if needed
    if needs_modification:
        img.save(temp_logo_path, 
                'PNG', 
                dpi=(DPI, DPI),  # Set the DPI metadata
                quality=95)      # High quality for PNG compression
    
    return temp_logo_path

def create_circle_pdf(logo_path, 
                     left_margin=LEFT_MARGIN*inch, 
                     right_margin=RIGHT_MARGIN*inch,
                     top_margin=TOP_MARGIN*inch, 
                     bottom_margin=BOTTOM_MARGIN*inch,
                     circle_diameter=CIRCLE_DIAMETER*inch,
                     num_columns=NUM_COLUMNS,
                     num_rows=NUM_ROWS,
                     oversize_factor=1.06):
    """
    Generate a PDF with evenly spaced circles and high-resolution logo overlays.
    """
    output_filename = get_output_filename(logo_path)
    try:
        circle_diameter *= oversize_factor

        # Prepare the logo
        temp_logo_path = prepare_logo(logo_path, CIRCLE_DIAMETER)
        
        # Set up the canvas with high quality settings
        c = canvas.Canvas(output_filename, pagesize=letter)
        # Set PDF metadata
        c.setTitle(f"Sticker Sheet - {os.path.basename(logo_path)}")
        c.setAuthor("Sticker Generator")
        
        width, height = letter
        circle_radius = circle_diameter / 2
        num_circles = num_columns * num_rows

        # Calculate available space
        available_width = width - left_margin - right_margin
        available_height = height - top_margin - bottom_margin

        # Calculate spacing between circles
        total_circle_width = num_columns * circle_diameter
        total_gaps_width = available_width - total_circle_width
        x_spacing = total_gaps_width / (num_columns - 1)

        total_circle_height = num_rows * circle_diameter
        total_gaps_height = available_height - total_circle_height
        y_spacing = total_gaps_height / (num_rows - 1)

        # Draw circles and logos
        for i in range(num_rows):
            for j in range(num_columns):
                if i * num_columns + j < num_circles:
                    # Calculate center positions
                    x = left_margin + (j * (circle_diameter + x_spacing))
                    y = height - (top_margin + (i * (circle_diameter + y_spacing)) + circle_diameter)
                    
                    # Draw circle with precise line width
                    c.setLineWidth(0.5)  # Set a thinner line for cleaner circles
                    c.circle(x + circle_radius, y + circle_radius, circle_radius, stroke=1, fill=0)
                    
                    # Draw logo with high quality settings
                    c.drawImage(temp_logo_path, 
                              x, y, 
                              width=circle_diameter, 
                              height=circle_diameter, 
                              mask='auto',
                              preserveAspectRatio=True)

        # Save the PDF with high quality compression
        c.setPageCompression(1)  # Enable PDF compression
        c.save()
        print(f"Generated {output_filename} at {DPI} DPI")
    
    finally:
        # Clean up the temporary file
        if 'temp_logo_path' in locals() and os.path.exists(temp_logo_path):
            os.remove(temp_logo_path)

def process_all_logos():
    """
    Process all logos in LOGO_PATHS and generate corresponding PDFs.
    """
    for logo_path in LOGO_PATHS:
        if os.path.exists(logo_path):
            print(f"\nProcessing {logo_path}:")
            create_circle_pdf(logo_path)
        else:
            print(f"Warning: Logo file not found: {logo_path}")

if __name__ == "__main__":
    # Generate PDF files for all logos
    process_all_logos()