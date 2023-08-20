class SPRTopMap:
    def __init__(self, contexts, cores, maps):
        self.data = [contexts, cores, maps]
        self.width = max(max(map(len, sublist)) for sublist in self.data) + 2  # Padding

    def _draw_line(self, content_list):
        """Return a formatted line for a given content list."""
        return "║" + "║".join(f" {item:{self.width}} " for item in content_list) + "║"

    def _draw_separator(self, is_top=False, is_bottom=False):
        """Return a formatted separator line."""
        corner_left = "╔" if is_top else ("╚" if is_bottom else "╠")
        corner_right = "╗" if is_top else ("╝" if is_bottom else "╣")
        middle = "╦" if is_top or is_bottom else "╬"
        line_piece = "═" * (self.width + 2)
        
        # Create segments for each column
        segments = [line_piece for _ in self.data[0]]
        
        # Intersperse segments with middle dividers
        result_parts = []
        for segment in segments[:-1]:
            result_parts.append(segment)
            result_parts.append(middle)
        result_parts.append(segments[-1])
        
        return corner_left + ''.join(result_parts) + corner_right

    def display(self):
        print(self._draw_separator(is_top=True))

        # Iterate through each section and print its content
        for section in self.data:
            for i, line in enumerate(section):
                # If this is not the first line of the section, add a thin separator
                if i > 0:
                    print(self._draw_separator())
                print(self._draw_line(line))

            # After each section (except the last), add a thick separator
            if section != self.data[-1]:
                print(self._draw_separator())

        print(self._draw_separator(is_bottom=True))


if __name__ == "__main__":
    contexts = [["context1a", "context2a", "context3a"],
                ["context1b", "context2b", "context3b"]]
    cores = [["core1a", "core2a", "core3a"],
             ["core1b", "core2b", "core3b"]]
    maps = [["map1a", "map2a", "map3a"],
            ["map1b", "map2b", "map3b"]]

    cpu_map = SPRTopMap(contexts, cores, maps)
    cpu_map.display()