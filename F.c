#include <cairo.h>
#include <gtk/gtk.h>

void drawImage(cairo_t *cr, const char *imagePath, double pageWidth, double pageHeight) {
    cairo_surface_t *image = cairo_image_surface_create_from_png(imagePath);

    if (cairo_surface_status(image) != CAIRO_STATUS_SUCCESS) {
        fprintf(stderr, "Error loading image: %s\n", imagePath);
        return;
    }

    double imageWidth = cairo_image_surface_get_width(image);
    double imageHeight = cairo_image_surface_get_height(image);

    double imageAspect = imageHeight / imageWidth;

    double printWidth = pageWidth;
    double printHeight = pageWidth * imageAspect;

    cairo_scale(cr, 1.0, -1.0);  // Invert the Y-axis
    cairo_translate(cr, 0, -pageHeight);  // Move the origin to the bottom-left corner

    cairo_set_source_surface(cr, image, pageWidth - printWidth, pageHeight - printHeight);
    cairo_paint(cr);

    cairo_surface_destroy(image);
}

void generatePDF(const char *outputFileName, const char *srcFolder) {
    cairo_surface_t *pdfSurface = cairo_pdf_surface_create(outputFileName, 595.276, 841.890);  // A4 page size in points
    cairo_t *cr = cairo_create(pdfSurface);

    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);  // White background
    cairo_paint(cr);

    char filePath[256];
    sprintf(filePath, "%s/*.png", srcFolder);

    // Add your logic to iterate through files in the folder and call drawImage for each image
    // You may need to use a library like dirent.h or a system call to list files in the directory

    drawImage(cr, "example.png", 595.276, 841.890);  // Example image, replace with actual logic

    cairo_surface_destroy(pdfSurface);
    cairo_destroy(cr);
}

int main(int argc, char *argv[]) {
    const char *outputFileName = "out.pdf";
    const char *srcFolder = (argc > 1) ? argv[1] : ".";

    gtk_init(&argc, &argv);

    generatePDF(outputFileName, srcFolder);

    return 0;
}
